#ifndef SIMULATION_BCH_HPP_
#define SIMULATION_BCH_HPP_

#include "Tools/Math/Galois.hpp"

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_BCH : public Simulation_BFER<B,R,Q>
{
protected:
	const tools::Galois GF;

public:
	Simulation_BFER_BCH(const tools::parameters& params);

	virtual ~Simulation_BFER_BCH();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_BCH_HPP_ */
