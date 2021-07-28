#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <sstream>
#include <numeric>
#include <string>
#include <limits>
#include <cmath>
#include <queue>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"
#include "Tools/Code/Polar/mc_func.hpp"
#include "Tools/Code/Polar/fb_extract.h"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_mcfast_sys.hpp"



namespace aff3ct
{
namespace module
{

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::Decoder_polar_SCL_mcfast_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits)
: Decoder_SIHO<B,R>(K, N),
  m                ((int)std::log2(N)),
  L                (L),
  frozen_bits      (frozen_bits),
  polar_patterns   (frozen_bits,
                    {new tools::Pattern_polar_std,
                     new tools::Pattern_polar_r0,
                     new tools::Pattern_polar_r1,
                     new tools::Pattern_polar_r0_left,
                     new tools::Pattern_polar_rep_left,
                     new tools::Pattern_polar_rep,       // /!\ perf. degradation with REP nodes in fixed-point
                    //  new tools::Pattern_polar_spc(2,2)}, // /!\ perf. degradation with SPC nodes length > 4 (when L is big)
					new tools::Pattern_polar_spc}, // /!\ perf. degradation with SPC nodes length > 4 (when L is big)
                    1,
                    2,
                    true),
  paths            (L),
  metrics          (L),
  l                (L, mipp::vector<R>(N + mipp::nElReg<R>())),
  s                (L, mipp::vector<B>(N + mipp::nElReg<B>())),
  metrics_vec      (3, std::vector<R>()),
  dup_count        (L, 0),
  bit_flips	       (L * L), // bits indices to be flipped * L paths
  is_even          (L),
  best_path        (0),
  n_active_paths   (1),
  n_array_ref      (L, std::vector<int>(m)),
  path_2_array     (L, std::vector<int>(m)),
  sorter           (N),
  best_idx         (L),
  l_tmp            (N)
{
	const std::string name = "Decoder_polar_SCL_mcfast_sys";
	this->set_name(name);
	this->set_n_frames_per_wave(API_polar::get_n_frames());

	std::cout << "mc decoder" << std::endl;

	static_assert(sizeof(B) == sizeof(R), "Sizes of the bits and reals have to be identical.");
//	static_assert(API_polar::get_n_frames() == 1, "The inter-frame API_polar is not supported.");

	if (API_polar::get_n_frames() != 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The inter-frame API_polar is not supported.");

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->L <= 0 || !tools::is_power_of_2(this->L))
	{
		std::stringstream message;
		message << "'L' has to be a positive power of 2 ('L' = " << L << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N < mipp::nElReg<R>() * 2)
	{
		std::stringstream message;
		message << "'N' has to be equal or greater than 'mipp::nElReg<R>()' * 2 ('N' = " << N
		        << ", 'mipp::nElReg<R>()' = " << mipp::nElReg<R>() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	metrics_vec[0].resize(L * 2);
	switch (L)
	{
		case 2:
			r1_mc_size  = {2};
			spc_mc_size = {2};
			metrics_vec[1].resize(L * r1_mc_size[0]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[0]);
			break;
		case 4:
			r1_mc_size  = {4, 5};
			spc_mc_size = {2, 5};
			metrics_vec[1].resize(L * r1_mc_size[1]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[1]);
			break;
		case 8:
			r1_mc_size  = {4, 10, 13};
			spc_mc_size = {2, 8,  13};
			metrics_vec[1].resize(L * r1_mc_size[2]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[2]);
			break;
		case 16:
			r1_mc_size  = {4, 16, 29, 36};
			spc_mc_size = {2, 8,  27, 36};
			metrics_vec[1].resize(L * r1_mc_size[3]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[3]);
			break;
		case 32:
			r1_mc_size  = {4, 16, 59, 80, 95};
			spc_mc_size = {2, 8,  53, 78, 95};
			metrics_vec[1].resize(L * r1_mc_size[4]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[4]);
			break;
		case 64:
			r1_mc_size  = {4, 16,  112, 186, 226, 258};
			spc_mc_size = {2, 8,   93,  182, 225, 258};
			metrics_vec[1].resize(L * r1_mc_size[5]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[5]);
			break;
		default:
			metrics_vec[1].resize(L * 4); // Chase-II
			metrics_vec[2].resize((L <= 2 ? 4 : 8) * L);
			break;
	}	
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::Decoder_polar_SCL_mcfast_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
                             const std::vector<tools::Pattern_polar_i*> &polar_patterns,
                             const int idx_r0, const int idx_r1)
: Decoder_SIHO<B,R>(K, N),
  m                ((int)std::log2(N)),
  L                (L),
  frozen_bits      (frozen_bits),
  polar_patterns   (frozen_bits, polar_patterns, idx_r0, idx_r1),
  paths            (L),
  metrics          (L),
  l                (L, mipp::vector<R>(N + mipp::nElReg<R>())),
  s                (L, mipp::vector<B>(N + mipp::nElReg<B>())),
  metrics_vec      (3, std::vector<R>()),
  dup_count        (L, 0),
  bit_flips        (L * L),
  is_even          (L),
  best_path        (0),
  n_active_paths   (1),
  n_array_ref      (L, std::vector<int>(m)),
  path_2_array     (L, std::vector<int>(m)),
  sorter           (N),
//sorter_simd      (N),
  best_idx         (L),
  l_tmp            (N)
{
	const std::string name = "Decoder_polar_SCL_mcfast_sys";
	this->set_name(name);
	this->set_n_frames_per_wave(API_polar::get_n_frames());

	static_assert(sizeof(B) == sizeof(R), "Sizes of the bits and reals have to be identical.");
//	static_assert(API_polar::get_n_frames() == 1, "The inter-frame API_polar is not supported.");

	if (API_polar::get_n_frames() != 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The inter-frame API_polar is not supported.");

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->L <= 0 || !tools::is_power_of_2(this->L))
	{
		std::stringstream message;
		message << "'L' has to be a positive power of 2 ('L' = " << L << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N < mipp::nElReg<R>() * 2)
	{
		std::stringstream message;
		message << "'N' has to be equal or greater than 'mipp::nElReg<R>()' * 2 ('N' = " << N
		        << ", 'mipp::nElReg<R>()' = " << mipp::nElReg<R>() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	metrics_vec[0].resize(L * 2);
	switch (L)
	{
		case 2:
			r1_mc_size  = {2};
			spc_mc_size = {2};
			metrics_vec[1].resize(L * r1_mc_size[0]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[0]);
			break;
		case 4:
			r1_mc_size  = {4, 5};
			spc_mc_size = {2, 5};
			metrics_vec[1].resize(L * r1_mc_size[1]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[1]);
			break;
		case 8:
			r1_mc_size  = {4, 10, 13};
			spc_mc_size = {2, 8,  13};
			metrics_vec[1].resize(L * r1_mc_size[2]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[2]);
			break;
		case 16:
			r1_mc_size  = {4, 16, 29, 36};
			spc_mc_size = {2, 8,  27, 36};
			metrics_vec[1].resize(L * r1_mc_size[3]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[3]);
			break;
		case 32:
			r1_mc_size  = {4, 16, 59, 80, 95};
			spc_mc_size = {2, 8,  53, 78, 95};
			metrics_vec[1].resize(L * r1_mc_size[4]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[4]);
			break;
		case 64:
			r1_mc_size  = {4, 16,  112, 186, 226, 258};
			spc_mc_size = {2, 8,   93,  182, 225, 258};
			metrics_vec[1].resize(L * r1_mc_size[5]); // L * |C|
			metrics_vec[2].resize(L * spc_mc_size[5]);
			break;
		default:
			metrics_vec[1].resize(L * 4); // Chase-II
			metrics_vec[2].resize((L <= 2 ? 4 : 8) * L);
			break;
	}
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::~Decoder_polar_SCL_mcfast_sys()
{
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_mcfast_sys<B,R,API_polar>* Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::clone() const
{
	auto m = new Decoder_polar_SCL_mcfast_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::set_frozen_bits(const std::vector<bool>& fb)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	polar_patterns.set_frozen_bits(fb);
}

template <typename B, typename R, class API_polar>
const std::vector<bool>& Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::init_buffers()
{
	metrics[0] = std::numeric_limits<R>::min();
	std::iota(paths.begin(), paths.begin() + L, 0);

	n_active_paths = 1;

	// at the beginning, path 0 points to array 0
	std::fill(n_array_ref [0].begin(), n_array_ref [0].end(), 1);
	std::fill(path_2_array[0].begin(), path_2_array[0].end(), 0);

	for (auto i = 1; i < L; i++)
		std::fill(n_array_ref[i].begin(), n_array_ref[i].end(), 0);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_decode(const R *Y_N)
{
	int first_node_id = 0, off_l = 0, off_s = 0;
	recursive_decode(Y_N, off_l, off_s, m, first_node_id);
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	if (!API_polar::isAligned(Y_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N' is misaligned memory.");

	if (!API_polar::isAligned(V_K))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'V_K' is misaligned memory.");

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->init_buffers();
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N);
	this->select_best_path(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	if (!API_polar::isAligned(Y_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N' is misaligned memory.");

	if (!API_polar::isAligned(V_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'V_N' is misaligned memory.");

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->init_buffers();
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N);
	this->select_best_path(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store_cw(V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::recursive_decode(const R *Y_N, const int off_l, const int off_s, const int rev_depth, int &node_id)
{
	const int n_elmts = 1 << rev_depth;
	const int n_elm_2 = n_elmts >> 1;
	const auto node_type = polar_patterns.get_node_type(node_id);

	const bool is_terminal_pattern = (node_type == tools::polar_node_t::RATE_0) ||
	                                 (node_type == tools::polar_node_t::RATE_1) ||
	                                 (node_type == tools::polar_node_t::REP)    ||
	                                 (node_type == tools::polar_node_t::SPC);

	// root node
	if (rev_depth == m)
	{
		// f
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				API_polar::f(Y_N, Y_N + n_elm_2, l[0].data(), n_elm_2);
				break;
			case tools::polar_node_t::REP_LEFT:
				API_polar::f(Y_N, Y_N + n_elm_2, l[0].data(), n_elm_2);
				break;
			default:
				break;
		}

		recursive_decode(Y_N, off_l, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g (Y_N, Y_N + n_elm_2, s[path].data() + off_s, child, n_elm_2);
				}
				break;
			case tools::polar_node_t::RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g0(Y_N, Y_N + n_elm_2,                         child, n_elm_2);
				}
				break;
			case tools::polar_node_t::REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::gr(Y_N, Y_N + n_elm_2, s[path].data() + off_s, child, n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(Y_N, off_l, off_s + n_elm_2, rev_depth -1, ++node_id); // recursive call right

		// xor
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			case tools::polar_node_t::RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2);
				break;
			case tools::polar_node_t::REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			default:
				break;
		}
	}
	else if (!is_terminal_pattern && rev_depth) // other node (not root or leaf)
	{
		// f
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case tools::polar_node_t::REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case tools::polar_node_t::RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths && n_active_paths > 1; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(Y_N, off_l + n_elmts, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g (parent + off_l, parent + off_l + n_elm_2, s[path].data() + off_s, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case tools::polar_node_t::RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g0(parent + off_l, parent + off_l + n_elm_2,                         child + off_l + n_elmts, n_elm_2);
				}
				break;
			case tools::polar_node_t::REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::gr(parent + off_l, parent + off_l + n_elm_2, s[path].data() + off_s, child + off_l + n_elmts, n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(Y_N, off_l + n_elmts, off_s + n_elm_2, rev_depth -1, ++node_id); // recursive call right

		// xor
		switch (node_type)
		{
			case tools::polar_node_t::STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			case tools::polar_node_t::RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2);
				break;
			case tools::polar_node_t::REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			default:
				break;
		}
	}
	else // leaf node
	{
		// h
		switch (node_type)
		{
			case tools::polar_node_t::RATE_0: update_paths_r0 (rev_depth, off_l, off_s, n_elmts); break;
			case tools::polar_node_t::REP:    update_paths_rep(rev_depth, off_l, off_s, n_elmts); break;
			case tools::polar_node_t::RATE_1: update_paths_r1 (rev_depth, off_l, off_s, n_elmts); break;
			case tools::polar_node_t::SPC:    update_paths_spc(rev_depth, off_l, off_s, n_elmts); break;
			default:
				break;
		}

		normalize_scl_metrics<R>(this->metrics, this->L);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_store(B *V_K) const
{
	tools::fb_extract(this->polar_patterns.get_leaves_pattern_types(), this->s[best_path].data(), V_K);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_store_cw(B *V_N) const
{
	std::copy(this->s[best_path].data(), this->s[best_path].data() + this->N, V_N);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_r0(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	if (n_active_paths > 1)
		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][r_d];

			auto pen = (R)0;
			for (auto j = 0; j < n_elmts; j++)
				pen = sat_m<R>(pen + sat_m<R>(-std::min((R)l[array][off_l +j], (R)0)));
			metrics[path] = sat_m<R>(metrics[path] + pen); // add a penalty to the current path metric
		}

//	if (!polar_patterns.exist_node_type(polar_node_t::RATE_0_LEFT, r_d +1))
		for (auto i = 0; i < n_active_paths; i++)
			API_polar::h0(s[paths[i]], off_s, n_elmts);
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_r0(const int off_l, const int off_s)
{
	if (n_active_paths > 1)
		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][REV_D];

			auto pen = (R)0;
			for (auto j = 0; j < N_ELMTS; j++)
				pen = sat_m<R>(pen + sat_m<R>(-std::min((R)l[array][off_l +j], (R)0)));
			metrics[path] = sat_m<R>(metrics[path] + pen); // add a penalty to the current path metric
		}

//	if (!polar_patterns.exist_node_type(polar_node_t::RATE_0_LEFT, REV_D +1))
		for (auto i = 0; i < n_active_paths; i++)
			API_polar::template h0<N_ELMTS>(s[paths[i]], off_s, N_ELMTS);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_r1(const int r_d, const int off_l, const int off_s, const int n_elmts)
{

	if (r_d == 0)
	{
		update_paths_rep(r_d, off_l, off_s, n_elmts);
	}
	else
	{
		auto bits_num = (n_elmts >= L) ? L -1                  : n_elmts;
		auto indice_r = (n_elmts >= L) ? (int)std::log2(L) - 1 : (int)std::log2(n_elmts) - 1;
		auto c_num    = r1_mc_size[(int)std::log2((n_elmts > L) ? L : n_elmts) - 1];
		std ::vector<R> pen(bits_num);


		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][r_d];			

			for (auto j = 0; j < n_elmts; j++) l_tmp[j] = std::abs(l[array][off_l + j]);
			_partial_sort(l_tmp.data(), best_idx, n_elmts, bits_num);
			for (int i = 0; i < bits_num; i++)
			{
				bit_flips[bits_num * path + i] = best_idx[i];
				pen[i] = sat_m<R>(std::abs(l[array][off_l + bit_flips[bits_num * path + i]]));
			} 
			update_r1_vec(path, n_elmts, L, c_num, pen, metrics, metrics_vec);
		}
		const auto n_list = (n_active_paths * r1_mc_size[indice_r] >= L) ? L : n_active_paths * r1_mc_size[indice_r];

		_partial_sort(metrics_vec[1].data(), best_idx, n_active_paths * r1_mc_size[indice_r], n_list);

		// count the number of duplications per path, count which old_path survive
		for (auto i = 0; i < n_list; i++)
			dup_count[best_idx[i] / r1_mc_size[indice_r]]++;

		// erase bad paths
		erase_bad_paths();

		for (auto i = 0; i < n_list; i++)
		{
			const auto path  = best_idx[i] / r1_mc_size[indice_r];
			const auto dup   = best_idx[i] % r1_mc_size[indice_r];
			const auto array = path_2_array[path][r_d];
			API_polar::h(s[path], l[array], off_l, off_s, n_elmts);
			const auto new_path = (dup_count[path] > 1) ? duplicate_tree(path, off_l, off_s, n_elmts) : path;
			flip_bits_r1(path, new_path, dup, off_s, n_elmts, bits_num, L, s, bit_flips);
			metrics[new_path] = metrics_vec[1][best_idx[i]];
			dup_count[path]--;
		}
	}
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_r1(const int off_l, const int off_s)
{

	if (REV_D == 0)
	{
		update_paths_rep(REV_D, off_l, off_s, N_ELMTS);
	}
	else
	{
		auto bits_num = (N_ELMTS >= L) ? L -1                  : N_ELMTS;
		auto indice_r = (N_ELMTS >= L) ? (int)std::log2(L) - 1 : (int)std::log2(N_ELMTS) - 1;
		auto c_num    = r1_mc_size[(int)std::log2((N_ELMTS > L) ? L : N_ELMTS) - 1];
		std ::vector<R> pen(bits_num);


		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][REV_D];			

			for (auto j = 0; j < N_ELMTS; j++) l_tmp[j] = std::abs(l[array][off_l + j]);
			_partial_sort(l_tmp.data(), best_idx, N_ELMTS, bits_num);
			for (int i = 0; i < bits_num; i++)
			{
				bit_flips[bits_num * path + i] = best_idx[i];
				pen[i] = sat_m<R>(std::abs(l[array][off_l + bit_flips[bits_num * path + i]]));
			} 
			update_r1_vec(path, N_ELMTS, L, c_num, pen, metrics, metrics_vec);
		}
		const auto n_list = (n_active_paths * r1_mc_size[indice_r] >= L) ? L : n_active_paths * r1_mc_size[indice_r];

		_partial_sort(metrics_vec[1].data(), best_idx, n_active_paths * r1_mc_size[indice_r], n_list);

		// count the number of duplications per path, count which old_path survive
		for (auto i = 0; i < n_list; i++)
			dup_count[best_idx[i] / r1_mc_size[indice_r]]++;

		// erase bad paths
		erase_bad_paths();

		for (auto i = 0; i < n_list; i++)
		{
			const auto path  = best_idx[i] / r1_mc_size[indice_r];
			const auto dup   = best_idx[i] % r1_mc_size[indice_r];
			const auto array = path_2_array[path][REV_D];
			API_polar::template h<N_ELMTS>(s[path], l[array], off_l, off_s, N_ELMTS);
			const auto new_path = (dup_count[path] > 1) ? duplicate_tree(path, off_l, off_s, N_ELMTS) : path;
			flip_bits_r1(path, new_path, dup, off_s, N_ELMTS, bits_num, L, s, bit_flips);
			metrics[new_path] = metrics_vec[1][best_idx[i]];
			dup_count[path]--;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_rep(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	constexpr B b = tools::bit_init<B>();

	// generate the two possible candidates
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][r_d];

		auto pen0 = (R)0;
		auto pen1 = (R)0;
		for (auto j = 0; j < n_elmts; j++)
		{
			pen0 = sat_m<R>(pen0 + sat_m<R>(-std::min(l[array][off_l +j], (R)0)));
			pen1 = sat_m<R>(pen1 + sat_m<R>(+std::max(l[array][off_l +j], (R)0)));
		}
		metrics_vec[0][2 * path +0] = sat_m<R>(metrics[path] + pen0);
		metrics_vec[0][2 * path +1] = sat_m<R>(metrics[path] + pen1);
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
		{
			const auto path = paths[i];
			const auto new_path = duplicate_tree(path, off_l, off_s, n_elmts);

			std::fill(s[    path].begin() + off_s, s[    path].begin() + off_s + n_elmts, 0);
			std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + n_elmts, b);

			metrics[    path] = metrics_vec[0][2 * path +0];
			metrics[new_path] = metrics_vec[0][2 * path +1];
		}
	}
	else // n_active_paths == L
	{
		// sort hypothetic metrics
		sorter.partial_sort(metrics_vec[0].data(), best_idx, L * 2, L);

		// count the number of duplications per path
		for (auto i = 0; i < L; i++)
			dup_count[best_idx[i] / 2]++;

		// erase bad paths
		erase_bad_paths();

		// duplicate paths
		for (auto path = 0; path < L; path++)
		{
			if (dup_count[path] == 1)
			{
				const auto comp = metrics_vec[0][2 * path +0] > metrics_vec[0][2 * path +1];
				std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, comp ? b : 0);

				metrics[path] = metrics_vec[0][2 * path + (comp ? 1 : 0)];
			}
			else if (dup_count[path] == 2)
			{
				const auto new_path = duplicate_tree(path, off_l, off_s, n_elmts);
				std::fill(s[    path].begin() + off_s, s[    path].begin() + off_s + n_elmts, 0);
				std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + n_elmts, b);

				metrics[    path] = metrics_vec[0][2 * path +0];
				metrics[new_path] = metrics_vec[0][2 * path +1];
			}

			dup_count[path] = 0;
		}
	}
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_rep(const int off_l, const int off_s)
{
	constexpr B b = tools::bit_init<B>();

	// generate the two possible candidates
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][REV_D];

		auto pen0 = (R)0;
		auto pen1 = (R)0;
		for (auto j = 0; j < N_ELMTS; j++)
		{
			pen0 = sat_m<R>(pen0 + sat_m<R>(-std::min(l[array][off_l +j], (R)0)));
			pen1 = sat_m<R>(pen1 + sat_m<R>(+std::max(l[array][off_l +j], (R)0)));
		}
		metrics_vec[0][2 * path +0] = sat_m<R>(metrics[path] + pen0);
		metrics_vec[0][2 * path +1] = sat_m<R>(metrics[path] + pen1);
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
		{
			const auto path = paths[i];
			const auto new_path = duplicate_tree(path, off_l, off_s, N_ELMTS);

			std::fill(s[    path].begin() + off_s, s[    path].begin() + off_s + N_ELMTS, 0);
			std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + N_ELMTS, b);

			metrics[    path] = metrics_vec[0][2 * path +0];
			metrics[new_path] = metrics_vec[0][2 * path +1];
		}
	}
	else // n_active_paths == L
	{
		// sort hypothetic metrics
		sorter.partial_sort(metrics_vec[0].data(), best_idx, L * 2, L);

		// count the number of duplications per path
		for (auto i = 0; i < L; i++)
			dup_count[best_idx[i] / 2]++;

		// erase bad paths
		erase_bad_paths();

		// duplicate paths
		for (auto path = 0; path < L; path++)
		{
			if (dup_count[path] == 1)
			{
				const auto comp = metrics_vec[0][2 * path +0] > metrics_vec[0][2 * path +1];
				std::fill(s[path].begin() + off_s, s[path].begin() + off_s + N_ELMTS, comp ? b : 0);

				metrics[path] = metrics_vec[0][2 * path + (comp ? 1 : 0)];
			}
			else if (dup_count[path] == 2)
			{
				const auto new_path = duplicate_tree(path, off_l, off_s, N_ELMTS);
				std::fill(s[    path].begin() + off_s, s[    path].begin() + off_s + N_ELMTS, 0);
				std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + N_ELMTS, b);

				metrics[    path] = metrics_vec[0][2 * path +0];
				metrics[new_path] = metrics_vec[0][2 * path +1];
			}

			dup_count[path] = 0;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_spc(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	auto bits_num = (n_elmts >= L) ? L                     : n_elmts;
	auto indice_r = (n_elmts >= L) ? (int)std::log2(L) - 1 : (int)std::log2(n_elmts) - 1;
	auto c_num = spc_mc_size[(int)std::log2((n_elmts > L) ? L : n_elmts) - 1];	
	std ::vector<R> pen(bits_num);


	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][r_d];		

		for (auto j = 0; j < n_elmts; j++) l_tmp[j] = std::abs(l[array][off_l + j]);
		_partial_sort(l_tmp.data(), best_idx, n_elmts, bits_num);
		for (auto i = 0; i < bits_num; i++)
		{
			bit_flips[bits_num * path + i] = best_idx[i];
			pen[i] = sat_m<R>(std::abs(l[array][off_l + bit_flips[bits_num * path + i]]));
		}
		
		auto sum = 0;

		for (auto i = 0; i < n_elmts; i++)
		{
			sum ^= (l[array][off_l + i] < 0);
		}	

		is_even[path] = (sum == 0);
		if (!is_even[path])
		{
			metrics[path] = metrics[path] + pen[0];
			pen[0] = -pen[0];
		}
		update_spc_vec(path, n_elmts, L, c_num, pen, metrics, metrics_vec);
	}
	const auto n_list = (n_active_paths * spc_mc_size[indice_r] >= L) ? L : n_active_paths * spc_mc_size[indice_r];

	_partial_sort(metrics_vec[2].data(), best_idx, n_active_paths * spc_mc_size[indice_r], n_list);

	// count the number of duplications per path, count which old_path survive
	for (auto i = 0; i < n_list; i++)
		dup_count[best_idx[i] / spc_mc_size[indice_r]]++;

	// erase bad paths
	erase_bad_paths();

	for (auto i = 0; i < n_list; i++)
	{
		const auto path  = best_idx[i] / spc_mc_size[indice_r];
		const auto dup   = best_idx[i] % spc_mc_size[indice_r];
		const auto array = path_2_array[path][r_d];
		API_polar::h(s[path], l[array], off_l, off_s, n_elmts);
		const auto new_path = (dup_count[path] > 1) ? duplicate_tree(path, off_l, off_s, n_elmts) : path;
		flip_bits_spc(path, new_path, dup, off_s, n_elmts, bits_num, L, is_even, s, bit_flips);
		// flip_bits_spc(path, new_path, dup, off_s, n_elmts, bits_num);
		metrics[new_path] = metrics_vec[2][best_idx[i]];
		dup_count[path]--;
	}
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::update_paths_spc(const int off_l, const int off_s)
{
	auto bits_num = (N_ELMTS >= L) ? L                     : N_ELMTS;
	auto indice_r = (N_ELMTS >= L) ? (int)std::log2(L) - 1 : (int)std::log2(N_ELMTS) - 1;
	auto c_num = spc_mc_size[(int)std::log2((N_ELMTS > L) ? L : N_ELMTS) - 1];	
	std ::vector<R> pen(bits_num);


	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][REV_D];		

		for (auto j = 0; j < N_ELMTS; j++) l_tmp[j] = std::abs(l[array][off_l + j]);
		_partial_sort(l_tmp.data(), best_idx, N_ELMTS, bits_num);
		for (auto i = 0; i < bits_num; i++)
		{
			bit_flips[bits_num * path + i] = best_idx[i];
			pen[i] = sat_f<R>(std::abs(l[array][off_l + bit_flips[bits_num * path + i]]));
		}
		
		auto sum = 0;

		for (auto i = 0; i < N_ELMTS; i++)
		{
			sum ^= (l[array][off_l + i] < 0);
		}	

		is_even[path] = (sum == 0);
		if (!is_even[path])
		{
			metrics[path] = metrics[path] + pen[0];
			pen[0] = -pen[0];
		}
		update_spc_vec(path, N_ELMTS, L, c_num, pen, metrics, metrics_vec);
		// _update_spc_vec(path, array, N_ELMTS, bits_num, off_l);	
	}
	const auto n_list = (n_active_paths * spc_mc_size[indice_r] >= L) ? L : n_active_paths * spc_mc_size[indice_r];


	_partial_sort(metrics_vec[2].data(), best_idx, n_active_paths * spc_mc_size[indice_r], n_list);

	// count the number of duplications per path, count which old_path survive
	for (auto i = 0; i < n_list; i++)
		dup_count[best_idx[i] / spc_mc_size[indice_r]]++;

	// erase bad paths
	erase_bad_paths();

	for (auto i = 0; i < n_list; i++)
	{
		const auto path  = best_idx[i] / spc_mc_size[indice_r];
		const auto dup   = best_idx[i] % spc_mc_size[indice_r];
		const auto array = path_2_array[path][REV_D];

		API_polar::template h<N_ELMTS>(s[path], l[array], off_l, off_s, N_ELMTS);

		const auto new_path = (dup_count[path] > 1) ? duplicate_tree(path, off_l, off_s, N_ELMTS) : path;
		flip_bits_spc(path, new_path, dup, off_s, N_ELMTS, bits_num, L, is_even, s, bit_flips);
		// flip_bits_spc(path, new_path, dup, off_s, N_ELMTS, bits_num);
		metrics[new_path] = metrics_vec[2][best_idx[i]];
		dup_count[path]--;
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::delete_path(int path_id)
{
	const auto old_path = paths[path_id];
	for (auto i = 0; i < m; i++)
		n_array_ref[path_2_array[old_path][i]][i]--;

	paths[path_id] = paths[--n_active_paths];
	paths[n_active_paths] = old_path;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::select_best_path(const size_t frame_id)
{
	best_path = -1;
	for (auto i = 0; i < n_active_paths; i++)
		if (best_path == -1 || metrics[paths[i]] < metrics[best_path])
			best_path = paths[i];

	if (best_path == -1)
		best_path = 0;

	return n_active_paths;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::up_ref_array_idx(const int path, const int r_d)
{
	auto old_array = path_2_array[path][r_d];

	// if more than 1 path points to the array
	if (n_array_ref[old_array][r_d] > 1)
	{
		// allocate new array to given path, r_d
		n_array_ref[old_array][r_d]--;

		auto new_array = 0;
		while (n_array_ref[new_array][r_d]) new_array++;

		path_2_array[path     ][r_d] = new_array;
		n_array_ref [new_array][r_d]++;

		return new_array;
	}

	return old_array;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::erase_bad_paths()
{
	// erase bad paths
	auto k = 0;
	auto n_active_paths_cpy = n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
		if (dup_count[paths[k]] == 0)
			delete_path(k);
		else
			k++;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::duplicate_tree(const int old_path, const int off_l, const int off_s, const int n_elmts)
{
	const auto new_path = paths[n_active_paths++];

	std::copy(path_2_array[old_path].begin(), path_2_array[old_path].end(), path_2_array[new_path].begin());

	for (auto i = 0; i < m; i++)
		n_array_ref[path_2_array[new_path][i]][i]++;

	std::copy(s[old_path].begin(), s[old_path].begin() + off_s + n_elmts, s[new_path].begin());

	return new_path;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_mcfast_sys<B,R,API_polar>
::_partial_sort(const R* values, std::vector<int> &pos, int n_elmts, int k)
{
    struct Node
    {
        float val;
        int idx;
        Node(float a=0, int b=0):
            val(a), idx(b) {}
    };

    struct cmp
    {
        bool operator()(Node a, Node b)
        {
            return a.val < b.val;
        }
    };

    std::priority_queue<Node, std::vector<Node>, cmp> p;

    for (int i = 0; i < k; ++i)
    {
        p.push(Node(values[i], i));
    } 

    for (int i = k; i < n_elmts; ++i)
    {
        if (p.top().val > values[i])
        {
            p.pop();
            p.push(Node(values[i], i));
        }
    }

    pos.resize(n_elmts);

    for (int i = k-1; i >= 0; --i)
    {
        pos[i] = p.top().idx;
        p.pop();
    }
}

}
}
