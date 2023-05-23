#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

struct WallCoordinates {
	WallCoordinates(glm::vec3 start, glm::vec3 end) : start(start), end(end) {
		//
	};

	glm::vec3 start;
	glm::vec3 end;
};

WallCoordinates roomCoords{
	glm::vec3(-150.0f, -40.0f, -150.0f),
	glm::vec3(450.0f, 60.0f, 450.0f),
};

std::vector<WallCoordinates> horizontalWallsCoords{
	WallCoordinates(
		glm::vec3(-150.0f, -40.0f, 149.0f),
		glm::vec3(-40.0f, 60.0f, 150.0f)
	),
	WallCoordinates(
		glm::vec3(40.0f, -40.0f, 149.0f),
		glm::vec3(260.0f, 60.0f, 150.0f)
	),
	WallCoordinates(
		glm::vec3(340.0f, -40.0f, 149.0f),
		glm::vec3(450.0f, 60.0f, 150.0f)
	)
};

std::vector<WallCoordinates> verticalWallsCoords{
	WallCoordinates(
		glm::vec3(149.0f, -40.0f, -150.0f),
		glm::vec3(150.0f, 60.0f, -40.0f)
	),
	WallCoordinates(
		glm::vec3(149.0f, -40.0f, 40.0f),
		glm::vec3(150.0f, 60.0f, 260.0f)
	),
	WallCoordinates(
		glm::vec3(149.0f, -40.0f, 340.0f),
		glm::vec3(150.0f, 60.0f, 450.0f)
	)
};

#endif // ROOM_H_INCLUDED