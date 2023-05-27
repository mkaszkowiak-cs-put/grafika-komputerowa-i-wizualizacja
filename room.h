#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

struct WallCoordinates {
	WallCoordinates(glm::vec3 start, glm::vec3 end) : start(start), end(end) {
		//
	};

	glm::vec3 start;
	glm::vec3 end;
};

struct PaintingSpotLight {
	PaintingSpotLight(glm::vec3 position, glm::vec3 direction) : position(position), direction(direction) {
		//
	};

	glm::vec3 position;
	glm::vec3 direction;
};

struct Painting : WallCoordinates {
	Painting(glm::vec3 start, glm::vec3 end, const char* texture, bool onZAxis) : WallCoordinates(start, end), texture(texture), spotLight(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)) {
		glm::vec3 lightPosition = glm::vec3(0, 0, 0);

		lightPosition.x = (start.x + end.x) / 2;
		lightPosition.y = end.y + 5.0f;
		lightPosition.z = (start.z + end.z) / 2;

		glm::vec3 lightDirection = glm::vec3(
			onZAxis ? 0.0f : lightPosition.x,
			-40.0f,
			!onZAxis ? 0.0f : lightPosition.z
		);

		spotLight = PaintingSpotLight(
			lightPosition,
			lightDirection
		);
	}

	const char* texture;
	PaintingSpotLight spotLight;
};

WallCoordinates roomCoords{
	glm::vec3(-150.0f, -40.0f, -150.0f),
	glm::vec3(450.0f, 60.0f, 450.0f),
};

std::vector<WallCoordinates> horizontalWallsCoords{
	WallCoordinates(
		glm::vec3(-150.0f, -40.0f, 147.5f),
		glm::vec3(-40.0f, 60.0f, 152.5f)
	),
	WallCoordinates(
		glm::vec3(40.0f, -40.0f, 147.5f),
		glm::vec3(260.0f, 60.0f, 152.5f)
	),
	WallCoordinates(
		glm::vec3(340.0f, -40.0f, 147.5f),
		glm::vec3(450.0f, 60.0f, 152.5f)
	)
};

std::vector<WallCoordinates> verticalWallsCoords{
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, -150.0f),
		glm::vec3(152.5f, 60.0f, -40.0f)
	),
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, 40.0f),
		glm::vec3(152.5f, 60.0f, 260.0f)
	),
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, 340.0f),
		glm::vec3(152.5f, 60.0f, 450.0f)
	)
};

std::vector<Painting> paintings {
	// ROOM 1
	Painting(
		glm::vec3(-30.0f, -20.0f, 447.0f),
		glm::vec3(30.0f, 40.0f, 449.0f),
		"paintings/burning-skull.png",
		true
	),
	Painting(
		glm::vec3(-147.0f, -20.0f, 270.0f),
		glm::vec3(-149.0f, 40.0f, 330.0f),
		"paintings/bust.png",
		false
	),
	// ROOM 2
	Painting(
		glm::vec3(270.0f, -20.0f, 447.0f),
		glm::vec3(330.0f, 40.0f, 449.0f),
		"paintings/match.png",
		true
	),
	Painting(
		glm::vec3(447.0f, -20.0f, 270.0f),
		glm::vec3(449.0f, 40.0f, 330.0f),
		"paintings/skull-and-roses.png",
		false
	),
	// ROOM 3
	Painting(
		glm::vec3(-37.5f, -20.0f, -147.0f),
		glm::vec3(37.5f, 40.0f, -149.0f),
		"paintings/skeleton.png",
		true
	),
	Painting(
		glm::vec3(-147.0f, -20.0f, -30.0f),
		glm::vec3(-149.0f, 40.0f, 30.0f),
		"paintings/stage.png",
		false
	),
	// ROOM 4
	Painting(
		glm::vec3(270.0f, -20.0f, -147.0f),
		glm::vec3(330.0f, 40.0f, -149.0f),
		"paintings/void.png",
		true
	),
	Painting(
		glm::vec3(447.0f, -20.0f, -30.0f),
		glm::vec3(449.0f, 40.0f, 30.0f),
		"paintings/pointer.png",
		false
	),
};

#endif // ROOM_H_INCLUDED