#include <iostream>

#include "common/toprsCommon.h"
#include "base/toprsIpt.h"


const toprs::IntFloatBitCoercion toprs::nanValue(~toprs_int64(0));

 double toprs::defaultMin(toprsDataType scalarType)
{
	switch(scalarType)
	{
	case TOPRS_UINT8:
		{
			return TOPRS_DEFAULT_MIN_PIX_UINT8;
		}
	case TOPRS_SINT8:
		{
			return TOPRS_DEFAULT_MIN_PIX_SINT8;
		}
	case TOPRS_UINT16:
		{
			return TOPRS_DEFAULT_MIN_PIX_UINT16;
		}
	case TOPRS_SINT16:
		{
			return TOPRS_DEFAULT_MIN_PIX_SINT16;
		}
	case TOPRS_USHORT11:
		{
			return TOPRS_DEFAULT_MIN_PIX_UINT11;
		}
	case TOPRS_UINT32:
		{
			return TOPRS_DEFAULT_MIN_PIX_UINT32;
		}
	case TOPRS_SINT32:
		{
			return TOPRS_DEFAULT_MIN_PIX_SINT32;
		}
	case TOPRS_FLOAT32:
		{
			return TOPRS_DEFAULT_MIN_PIX_FLOAT;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			return TOPRS_DEFAULT_MIN_PIX_NORM_FLOAT;
		}
	case TOPRS_FLOAT64:
		{
			return TOPRS_DEFAULT_MIN_PIX_DOUBLE;
		}
	case TOPRS_NORMALIZED_DOUBLE:
		{
			return TOPRS_DEFAULT_MIN_PIX_NORM_DOUBLE;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< __FILE__ << ":" << __LINE__
				<< "\nUnhandled scalar type:  " << scalarType << std::endl;
			break;
		}
	}
	return 0.0; // Should never happen...
}

 double toprs::defaultMax(toprsDataType scalarType)
{
	switch(scalarType)
	{
	case TOPRS_UINT8:
		{
			return TOPRS_DEFAULT_MAX_PIX_UINT8;
		}
	case TOPRS_SINT8:
		{
			return TOPRS_DEFAULT_MAX_PIX_SINT8;
		}
	case TOPRS_UINT16:
		{
			return TOPRS_DEFAULT_MAX_PIX_UINT16;
		}
	case TOPRS_SINT16:
		{
			return TOPRS_DEFAULT_MAX_PIX_SINT16;
		}
	case TOPRS_USHORT11:
		{
			return TOPRS_DEFAULT_MAX_PIX_UINT11;
		}
	case TOPRS_UINT32:
		{
			return TOPRS_DEFAULT_MAX_PIX_UINT32;
		}
	case TOPRS_SINT32:
		{
			return TOPRS_DEFAULT_MAX_PIX_SINT32;
		}
	case TOPRS_FLOAT32:
		{
			return TOPRS_DEFAULT_MAX_PIX_FLOAT;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			return TOPRS_DEFAULT_MAX_PIX_NORM_FLOAT;
		}
	case TOPRS_FLOAT64:
		{
			return TOPRS_DEFAULT_MAX_PIX_DOUBLE;
		}
	case TOPRS_NORMALIZED_DOUBLE:
		{
			return TOPRS_DEFAULT_MAX_PIX_NORM_DOUBLE;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< __FILE__ << ":" << __LINE__
				<< "\nUnhandled scalar type:  " << scalarType << std::endl;
			break;
		}
	}
	return 0.0; // Should never happen...
}

 double toprs::defaultNull(toprsDataType scalarType)
{
	switch(scalarType)
	{
	case TOPRS_UINT8:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT8;
		}
	case TOPRS_SINT8:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT8;
		}
	case TOPRS_UINT16:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT16;
		}
	case TOPRS_SINT16:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT16;
		}
	case TOPRS_USHORT11:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT11;
		}
	case TOPRS_UINT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT32;
		}
	case TOPRS_SINT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT32;
		}
	case TOPRS_FLOAT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_FLOAT;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			return TOPRS_DEFAULT_NULL_PIX_NORM_FLOAT;
		}
	case TOPRS_FLOAT64:
		{
			return TOPRS_DEFAULT_NULL_PIX_DOUBLE;
		}
	case TOPRS_NORMALIZED_DOUBLE:
		{
			return TOPRS_DEFAULT_NULL_PIX_NORM_DOUBLE;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< __FILE__ << ":" << __LINE__
				<< "\nUnhandled scalar type:  " << scalarType << std::endl;
			break;
		}
	}

	return 0.0; // Should never happen...
}

 toprs_uint32 toprs::dataSizeInBytes(toprsDataType scalarType)
{
	switch(scalarType)
	{
	case TOPRS_UINT8:
		{
			return sizeof(toprs_uint8);
		}
	case TOPRS_SINT8:
		{
			return sizeof(toprs_sint8);
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			return sizeof(toprs_uint16);
		}
	case TOPRS_SINT16:
		{
			return sizeof(toprs_sint16);
		}
	case TOPRS_UINT32:
		{
			return sizeof(toprs_uint32);
		}
	case TOPRS_SINT32:
		{
			return sizeof(toprs_sint32);
		}
	case TOPRS_FLOAT32:
	case TOPRS_NORMALIZED_FLOAT:
		{
			return sizeof(toprs_float32);
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			return sizeof(toprs_float64);
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< __FILE__ << ":" << __LINE__
				<< "\nUnhandled scalar type:  " << scalarType << std::endl;
			break;
		}
	}

	return 1;
}

template<class T>
 auto toprs::defaultNull(toprsDataType scalarType, T dummy) -> decltype(dummy)
{
	switch(scalarType)
	{
	case TOPRS_UINT8:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT8;
		}
	case TOPRS_SINT8:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT8;
		}
	case TOPRS_UINT16:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT16;
		}
	case TOPRS_SINT16:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT16;
		}
	case TOPRS_USHORT11:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT11;
		}
	case TOPRS_UINT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_UINT32;
		}
	case TOPRS_SINT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_SINT32;
		}
	case TOPRS_FLOAT32:
		{
			return TOPRS_DEFAULT_NULL_PIX_FLOAT;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			return TOPRS_DEFAULT_NULL_PIX_NORM_FLOAT;
		}
	case TOPRS_FLOAT64:
		{
			return TOPRS_DEFAULT_NULL_PIX_DOUBLE;
		}
	case TOPRS_NORMALIZED_DOUBLE:
		{
			return TOPRS_DEFAULT_NULL_PIX_NORM_DOUBLE;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< __FILE__ << ":" << __LINE__
				<< "\nUnhandled scalar type:  " << scalarType << std::endl;
			break;
		}
	}

	return 0.0; // Should never happen...
 }

  void toprs::defaultTileSize( toprsIpt& tileSize )
 {
	 tileSize.x = TOPRS_DEFAULT_TILE_WIDTH;
	 tileSize.y = TOPRS_DEFAULT_TILE_HEIGHT;
 }
