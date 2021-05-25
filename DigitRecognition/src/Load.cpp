#include "Load.h"

#include <fstream>

static uint8_t readUInt8(std::vector<uint8_t>& bytes, size_t& offset)
{
	return bytes[offset++];
}

static uint16_t readUInt16(std::vector<uint8_t>& bytes, size_t& offset)
{
	return bytes[offset++] << 8 | bytes[offset++];
}

static uint32_t readUInt32(std::vector<uint8_t>& bytes, size_t& offset)
{
	return bytes[offset++] << 24 | bytes[offset++] << 16 | bytes[offset++] << 8 | bytes[offset++];
}

void loadIDX(const std::string_view& filename, std::vector<std::pair<arma::fvec, arma::fvec>>& inputs)
{
	std::vector<uint8_t> imageBytes;
	std::vector<uint8_t> labelBytes;
	{
		std::ifstream images(std::string(filename) + "-images", std::ios::ate | std::ios::binary);
		if (images.is_open())
		{
			imageBytes.resize(images.tellg());
			images.seekg(0);
			images.read(reinterpret_cast<char*>(imageBytes.data()), imageBytes.size());
			images.close();
		}
	}
	{
		std::ifstream labels(std::string(filename) + "-labels", std::ios::ate | std::ios::binary);
		if (labels.is_open())
		{
			labelBytes.resize(labels.tellg());
			labels.seekg(0);
			labels.read(reinterpret_cast<char*>(labelBytes.data()), labelBytes.size());
			labels.close();
		}
	}

	size_t imageByteOffset = 0;
	size_t labelByteOffset = 0;
	if (readUInt16(imageBytes, imageByteOffset) == 0 && readUInt16(labelBytes, labelByteOffset) == 0)
	{
		uint8_t imageDataType       = readUInt8(imageBytes, imageByteOffset);
		uint8_t imageDimensionCount = readUInt8(imageBytes, imageByteOffset);
		uint8_t labelDataType       = readUInt8(labelBytes, labelByteOffset);
		uint8_t labelDimensionCount = readUInt8(labelBytes, labelByteOffset);
		if (imageDataType != 0x08 || labelDataType != 0x08 || imageDimensionCount != 3 || labelDimensionCount != 1)
			return;

		uint32_t imageDim1 = readUInt32(imageBytes, imageByteOffset);
		uint32_t labelDim1 = readUInt32(labelBytes, labelByteOffset);
		uint32_t numImages = std::min(imageDim1, labelDim1);
		inputs.resize(numImages);
		uint32_t imageWidth  = readUInt32(imageBytes, imageByteOffset);
		uint32_t imageHeight = readUInt32(imageBytes, imageByteOffset);
		for (size_t i = 0; i < numImages; i++)
		{
			arma::fvec image(static_cast<size_t>(imageWidth) * imageHeight);
			arma::fvec result { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			uint8_t resultNum = readUInt8(labelBytes, labelByteOffset);
			result[resultNum] = 1.0f;

			for (size_t x = 0; x < imageWidth; x++)
				for (size_t y = 0; y < imageHeight; y++)
					image[x + y * imageWidth] = readUInt8(imageBytes, imageByteOffset) / 255.0f;

			inputs[i] = { image, result };
		}
	}
}