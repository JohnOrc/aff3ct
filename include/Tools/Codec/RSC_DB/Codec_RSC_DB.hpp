/*!
 * \file
 * \brief Class tools::Codec_RSC_DB.
 */
#ifndef CODEC_RSC_DB_HPP_
#define CODEC_RSC_DB_HPP_

#include <vector>

#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Tools/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RSC_DB : public Codec_SISO_SIHO<B,Q>
{
protected:
	const bool buffered_encoding;
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC_DB(const factory::Encoder_RSC_DB &enc_params,
	             const factory::Decoder_RSC_DB &dec_params);
	virtual ~Codec_RSC_DB() = default;

	virtual Codec_RSC_DB<B,Q>* clone() const;

	bool is_buffered_encoding() const;
	const std::vector<std::vector<int>>& get_trellis() const;
};
}
}

#endif /* CODEC_RSC_DB_HPP_ */
