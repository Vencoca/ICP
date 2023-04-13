#include <opencv2\opencv.hpp>
#include "texture.h"

static void textureLoad(cv::Mat& image, const bool transparent, bool compressed = true)
{
	if (compressed && glewIsSupported("GL_ARB_texture_compression"))
	{
		GLint num_compressed_format;

		// get list of supported formats
		glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &num_compressed_format);
		if (num_compressed_format > 0)
		{
			GLint internalformat, compressed_size;
			int compressed;

			std::cout << "COMPRESSION supported, available formats: " << num_compressed_format << std::endl;

			// try to load compressed texture
			glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
			//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

			if (transparent)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ARB, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_ARB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
			}

			// is it really compressed?
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &compressed);
			/* if the compression has been successful */
			if (compressed == GL_TRUE)
			{
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, &compressed_size);
				std::cout << "ORIGINAL: " << image.total() * image.elemSize() << " COMPRESSED: " << compressed_size << " INTERNAL FORMAT: " << internalformat << std::endl;
			}
		}
	}
	else
	{
		// load uncompressed
		if (transparent)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
		}
	}
}

GLuint textureInit(const char* cesta, const bool mirror, const bool transparent)
{
	GLuint texture;
	cv::Mat image;

	if (transparent) {
		image = cv::imread(cesta, cv::IMREAD_UNCHANGED);
	}
	else {
		image = cv::imread(cesta);
	}

	if (image.empty())
	{
		std::cerr << "no texture: " << cesta << std::endl;
		exit(1);
	}

	glGenTextures(1, &texture);
	std::cout << "TexID: " << texture << std::endl;

	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	textureLoad(image, transparent, true);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (mirror) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // bilinear - better & slower
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
