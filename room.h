#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

struct WallCoordinates {
	WallCoordinates(glm::vec3 start, glm::vec3 size) : start(start), size(size) {
		//
	};

	glm::vec3 start;
	glm::vec3 size;
};

struct PaintingSpotLight {
	PaintingSpotLight(glm::vec3 position, glm::vec3 direction) : position(position), direction(direction) {
		//
	};

	glm::vec3 position;
	glm::vec3 direction;
};

struct Painting : WallCoordinates {
	Painting(glm::vec3 start, glm::vec3 size, const char* texture, bool onZAxis) : WallCoordinates(start, size), texture(texture), spotLight(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)) {
		glm::vec3 lightPosition = glm::vec3(150, -20, 150);

		auto centerX = start.x + (size.x / 2);
		auto topY = (start.y + size.y) + 5.0f;
		auto centerZ = start.z + (size.z / 2);

		auto pointTo = glm::vec3(start.x, start.y, start.z);

		printf("[Light] Painting at X: %.2f Y: %.2f Z: %.2f \n", start.x, start.y, start.z);

		glm::vec3 lightDirection = glm::normalize(pointTo - lightPosition);

		spotLight = PaintingSpotLight(
			lightPosition,
			lightDirection
		);
	}

	const char* texture;
	PaintingSpotLight spotLight;
};

std::vector<WallCoordinates> roomWallsCoords{
	WallCoordinates(
		glm::vec3(-154.5f, -40.0f, -150.0f),
		glm::vec3(5.f, 100.f, 601.f)
	),
	WallCoordinates(
		glm::vec3(450.5f, -40.0f, -149.0f),
		glm::vec3(5.f, 100.f, 419.0f)
	),
	WallCoordinates(
		glm::vec3(450.5f, -40.0f, 330.0f),
		glm::vec3(5.f, 100.f, 120.0f)
	),
	WallCoordinates(
		glm::vec3(-149.5f, -40.0f, 450.0f),
		glm::vec3(601.0f, 100.0f, 5.0f)
	),
	WallCoordinates(
		glm::vec3(-150.5f, -40.0f, -154.0f),
		glm::vec3(601.0f, 100.0f, 5.0f)
	),
};

std::vector<WallCoordinates> horizontalWallsCoords{
	WallCoordinates(
		glm::vec3(-150.0f, -40.0f, 147.5f),
		glm::vec3(110.f, 100.f, 5.0f)
	),
	WallCoordinates(
		glm::vec3(40.0f, -40.0f, 147.5f),
		glm::vec3(220.f, 100.f, 5.0f)
	),
	WallCoordinates(
		glm::vec3(340.0f, -40.0f, 147.5f),
		glm::vec3(110.f, 100.f, 5.0f)
	)
};

std::vector<WallCoordinates> verticalWallsCoords{
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, -150.0f),
		glm::vec3(5.0f, 100.f, 110.0f)
	),
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, 40.0f),
		glm::vec3(5.0f, 100.f, 220.0f)
	),
	WallCoordinates(
		glm::vec3(147.5f, -40.0f, 340.0f),
		glm::vec3(5.0f, 100.f, 110.0f)
	)
};

std::vector<Painting> paintings {
	// ROOM 1
	Painting(
		glm::vec3(-30.0f, -20.0f, 447.0f),
		glm::vec3(60.0f, 60.0f, 2.0f),
		"paintings/burning-skull.png",
		true
	),
	
	Painting(
		glm::vec3(-149.0f, -20.0f, 270.0f),
		glm::vec3(2.0f, 60.0f, 60.0f),
		"paintings/bust.png",
		false
	),
	// ROOM 2
	Painting(
		glm::vec3(270.0f, -20.0f, 447.0f),
		glm::vec3(60.0f, 60.0f, 2.0f),
		"paintings/match.png",
		true
	),
/*
	Painting(
		glm::vec3(447.0f, -20.0f, 270.0f),
		glm::vec3(2.0f, 60.0f, 60.0f),
		"paintings/skull-and-roses.png",
		false
	), // blocks the door
	*/ 
	// ROOM 3
	Painting(
		glm::vec3(-37.5f, -20.0f, -149.0f),
		glm::vec3(75.0f, 60.0f, 2.0f),
		"paintings/skeleton.png",
		true
	),
	Painting(
		glm::vec3(-149.0f, -20.0f, -30.0f),
		glm::vec3(2.0f, 60.0f, 60.0f),
		"paintings/stage.png",
		false
	),
	// ROOM 4
	Painting(
		glm::vec3(270.0f, -20.0f, -149.0f),
		glm::vec3(60.0f, 60.0f, 2.0f),
		"paintings/void.png",
		true
	),
	Painting(
		glm::vec3(447.0f, -20.0f, -30.0f),
		glm::vec3(2.0f, 60.0f, 60.0f),
		"paintings/pointer.png",
		false
	),
};

#endif // ROOM_H_INCLUDED