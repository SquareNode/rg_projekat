a: includes/glad.c includes/stb_image.h includes/shader.h includes/shader.cpp proj.cpp
	g++ includes/glad.c includes/shader.cpp proj.cpp -lglfw3 -lopengl32 -lglu32 -lgdi32 -lassimp
	
	