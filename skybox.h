#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

class Skybox : public Drawable {
public:

	void draw() {
		glDepthFunc(GL_LEQUAL);
		shader->use();

		glm::mat4 V = glm::lookAt(
			glm::vec3(0, 0, 0),
			cameraDirection,
			cameraUp
		);

		glm::mat4 P = glm::perspective(glm::radians(50.0f), aspectRatio, 0.01f, 5000.0f); //Wylicz macierz rzutowania


		glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(V));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthFunc(GL_LESS);
		glBindVertexArray(0);
	}

	DrawablePriority getDrawablePriority() {
		return DrawablePriority::LOW;
	}

protected:
	void initDrawable() {
		create_cube_map(
			"textures/skybox/negz.jpg", // back - negz
			"textures/skybox/posz.jpg", // front - posz
			"textures/skybox/posy.jpg", // top - posy
			"textures/skybox/negy.jpg", // bottom - negy
			"textures/skybox/negx.jpg", // left - negx
			"textures/skybox/posx.jpg", // right - posx
			&texCube
		);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skybox_points, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	float skybox_points[108] = {
  -10.0f,  10.0f, -10.0f,
  -10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,

  -10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,

   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,

  -10.0f, -10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,

  -10.0f,  10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f, -10.0f,

  -10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f,  10.0f
	};

	GLuint vbo;
	GLuint vao;
	GLuint texCube;

	bool load_cube_map_side_png(GLuint texture, GLenum side_target, const char* file_name) {
		// Note: untested! I accidentally converted it to lodepng, leaving it in
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		unsigned x, y;
		std::vector<unsigned char> image;
		string filename = string(file_name);
		unsigned error = lodepng::decode(image, x, y, filename);
		if (error) {
			std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return false;
		}

		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
			fprintf(stderr,
				"WARNING: image %s is not power-of-2 dimensions\n",
				file_name);
		}

		// copy image data into 'target' side of cube map
		glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
		image.clear();
		return true;
	}

	bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int x, y, n;
		int force_channels = 4;
		unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
		if (!image_data) {
			fprintf(stderr, "ERROR: could not load %s\n", file_name);
			return false;
		}

		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
			fprintf(stderr,
				"WARNING: image %s is not power-of-2 dimensions\n",
				file_name);
		}

		// copy image data into 'target' side of cube map
		glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		free(image_data);
		return true;
	}

	void create_cube_map(
		const char* negz,
		const char* posz,
		const char* posy,
		const char* negy,
		const char* negx,
		const char* posx,
		GLuint* tex_cube) {
		// generate a cube-map texture to hold all the sides
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, tex_cube);

		// load each image and copy into a side of the cube-map texture
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negz);
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posz);
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posy);
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negy);
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negx);
		load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, posx);
		// format cube map texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
};







#endif // SKYBOX_H_INCLUDED