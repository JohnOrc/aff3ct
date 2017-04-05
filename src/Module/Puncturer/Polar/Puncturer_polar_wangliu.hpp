#ifndef PUNCTURER_POLAR_WANGLIU_HPP_
#define PUNCTURER_POLAR_WANGLIU_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_polar_wangliu : public Puncturer<B,Q>
{
protected:
	const tools::Frozenbits_generator<B> &fb_generator;

public:
	Puncturer_polar_wangliu(const int &K,
	                        const int &N,
	                        const tools::Frozenbits_generator<B> &fb_generator,
	                        const int n_frames = 1,
	                        const std::string name = "Puncturer_polar_wangliu");
	virtual ~Puncturer_polar_wangliu();

	void gen_frozen_bits(mipp::vector<B> &frozen_bits);

protected:
	void   _puncture_fbf(const B *X_N1, B *X_N2) const;
	void _depuncture_fbf(const Q *Y_N1, Q *Y_N2) const;
};
}
}

#endif /* PUNCTURER_POLAR_WANGLIU_HPP_ */
