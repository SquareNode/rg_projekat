#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "includes/shader.h"
#include "includes/camera.h"
#include "includes/mesh.h"
#include "includes/model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* w);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);

int load_texture(std::string path);

int width = 800, height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

glm::vec3 lightPos(1.0f, 3.0f, 5.0f);

float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

bool turn_on_off = false, move_scene = false;

int main () {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	int width = 800, height = 600;
	GLFWwindow* w = glfwCreateWindow(width,height, "LearnOpenGL", NULL, NULL);
	
	if(w == NULL){
		std::cout << "Couldn't make a window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(w);
	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
	glfwSetCursorPosCallback(w, mouse_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetKeyCallback(w, key_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Loading GLAD failed!" << std::endl;
		return -1;
	}
	
	float podloga_cvorovi[] = {
		 0.5f,	0.5f, 0.0f,	 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		-0.5f,	0.5f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f, 0.0f  // top left 
	};
	unsigned int podloga_ind[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};
	
	float cvorovi[] = {
		// positions		   // texture coords
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,
		0.5f,	0.5f, -0.5f,	1.0f,  1.0f,
		0.5f,	0.5f, -0.5f,	1.0f,  1.0f,
		-0.5f,	0.5f, -0.5f,  0.0f,	 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,	 0.0f,
		0.5f, -0.5f,  0.5f,	1.0f,  0.0f,
		0.5f,	0.5f,  0.5f,  1.0f,	 1.0f,
		0.5f,	0.5f,  0.5f,	1.0f,  1.0f,
		-0.5f,	0.5f,  0.5f,   0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,

		-0.5f,	0.5f,  0.5f,   1.0f,  0.0f,
		-0.5f,	0.5f, -0.5f,   1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,	 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,	 0.0f,
		-0.5f,	0.5f,  0.5f, 1.0f,	0.0f,

		0.5f,	0.5f,  0.5f,  1.0f,	 0.0f,
		0.5f,	0.5f, -0.5f,	1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,   0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,   0.0f,  0.0f,
		0.5f,	0.5f,  0.5f,   1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,	 1.0f,
		0.5f, -0.5f,  0.5f,1.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 1.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,	 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,	 1.0f,

		-0.5f,	0.5f, -0.5f,	0.0f,  1.0f,
		0.5f,	0.5f, -0.5f,  1.0f,	 1.0f,
		0.5f,	0.5f,  0.5f,  1.0f,	 0.0f,
		0.5f,	0.5f,  0.5f,  1.0f,	 0.0f,
		-0.5f,	0.5f,  0.5f, 0.0f,	0.0f,
		-0.5f,	0.5f, -0.5f,   0.0f,  1.0f
	};
	
	// float tet [] = {
		// -0.28868, 0.28868, -0.28868,	 0.57735, 0.57735, -0.57735,
		// 0.28868, -0.28868, -0.28868,	 0.57735, 0.57735, -0.57735,
		// 0.28868, 0.28868, 0.28868,	0.57735, 0.57735, -0.57735,
		
		// 0.28868, 0.28868, 0.28868, -0.57735, 0.57735, 0.57735,
		// -0.28868, -0.28868, 0.28868, -0.57735, 0.57735, 0.57735,
		// -0.28868, 0.28868, -0.28868,	 -0.57735, 0.57735, 0.57735,
		
		// -0.28868, 0.28868, -0.28868, -0.57735, -0.57735, -0.57735,
		// -0.28868, -0.28868, 0.28868, -0.57735, -0.57735, -0.57735,
		// 0.28868, -0.28868, -0.28868,-0.57735, -0.57735, -0.57735,
		
		// -0.28868, -0.28868, 0.28868, 0.57735, -0.57735, 0.57735,
		// 0.28868, 0.28868, 0.28868, 0.57735, -0.57735, 0.57735,
		// 0.28868, -0.28868, -0.28868, 0.57735, -0.57735, 0.57735
	// };
	
	// pokusaj pronalazenja tacnih normali, ne valja... 
	// float tet [] = {
		// -0.28868, 0.28868, -0.28868, -0.57735, -0.57735, 0.57735,
		// 0.28868, -0.28868, -0.28868, -0.57735, -0.57735, 0.57735,
		// 0.28868, 0.28868, 0.28868,	-0.57735, -0.57735, 0.57735,
		
		// 0.28868, 0.28868, 0.28868, -0.57735, 0.57735, 0.57735,
		// -0.28868, -0.28868, 0.28868, -0.57735, 0.57735, 0.57735,
		// -0.28868, 0.28868, -0.28868,	 -0.57735, 0.57735, 0.57735,
		
		// -0.28868, 0.28868, -0.28868, 0.57735, 0.57735, 0.57735,
		// -0.28868, -0.28868, 0.28868, 0.57735, 0.57735, 0.57735,
		// 0.28868, -0.28868, -0.28868,0.57735, 0.57735, 0.57735,
		
		// -0.28868, -0.28868, 0.28868,0.57735, -0.57735, 0.57735,
		// 0.28868, 0.28868, 0.28868, 0.57735, -0.57735, 0.57735,
		// 0.28868, -0.28868, -0.28868, 0.57735, -0.57735, 0.57735
	// };
	
	//dobijeno uz pomoc knjige iz geometrije
	float tet [] = {
		0.0, 0.0, 1.0, 0.382546, 0.883452, 0.270501,
		2*glm::sqrt(2) /3.0, 0, -1.0/3.0, 0.382546, 0.883452, 0.270501,
		-2*glm::sqrt(2) /3.0, glm::sqrt(6)/3.0, -1.0/3.0, 0.382546, 0.883452, 0.270501,
		
		0.0, 0.0, 1.0, -0.816497, 0, 0.57735,
		-2*glm::sqrt(2) /3.0, glm::sqrt(6)/3.0, -1.0/3.0, -0.816497, 0, 0.57735,
		-2*glm::sqrt(2) /3.0, -glm::sqrt(6)/3.0, -1.0/3.0, -0.816497, 0, 0.57735,
		
		0.0, 0.0, 1.0, 0.382546, -0.883452, 0.270501, 
		2*glm::sqrt(2) /3.0, 0, -1.0/3.0,0.382546, -0.883452, 0.270501, 
		-2*glm::sqrt(2) /3.0, -glm::sqrt(6)/3.0, -1.0/3.0,0.382546, -0.883452, 0.270501,
		
		2*glm::sqrt(2) /3.0, 0, -1.0/3.0, -0, 0, -1,
		-2*glm::sqrt(2) /3.0, glm::sqrt(6)/3.0, -1.0/3.0, -0, 0, -1, 
		-2*glm::sqrt(2) /3.0, -glm::sqrt(6)/3.0, -1.0/3.0, -0, 0, -1
		
	};
	/*
	
	int tet_ind [] = {
		1, 2, 0,
		0, 3, 1,
		1, 3, 2,
		3, 0, 2
	};
	
	int tet_nor_ind [] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3
	};
	
	float tet_4_cv [] = {
		0.28868, 0.28868, 0.28868,			//svi poz
		-0.28868, 0.28868, -0.28868,		//neg poz neg
		0.28868, -0.28868, -0.28868,		//poz neg neg
		-0.28868, -0.28868, 0.28868			//neg neg poz
	};
	float tet_4_norm [] = {
		0.57735, 0.57735, -0.57735, 
		-0.57735, 0.57735, 0.57735, 
		-0.57735, -0.57735, -0.57735,
		0.57735, -0.57735, 0.57735
	};
	
	*/
	
	Shader obj_shader("shaders/proj.vs", "shaders/proj.fs");
	Shader light_shader("shaders/light.vs", "shaders/light.fs");
	
	Model table("models/desk/Desk_OBJ.obj"); 
	Model lightbulb("models/light/uploads_files_2558223_Lightbulb.obj"); 
	
	Shader model_shader("shaders/model.vs", "shaders/model.fs");	
	Shader podloga_shader("shaders/podloga.vs", "shaders/podloga.fs");
	
	int tex = load_texture("images/some_text.jpg");
	int wojak_tex = load_texture("images/Wojak.jpg");

	unsigned int indexi[] = {
		 0, 1, 3, 
		1, 2, 3	 
	};
	
	//kocka
	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cvorovi), cvorovi, GL_STATIC_DRAW);
		
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),
	(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
	(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//tetraedar
	unsigned int VAO_tet;
	unsigned int VBO_tet;
	
	glGenVertexArrays(1, &VAO_tet);
	glGenBuffers(1, &VBO_tet);
	
	glBindVertexArray(VAO_tet);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tet);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tet), tet, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
	(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
	(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//podloga sa EBO
	unsigned int VAO_pod, VBO_pod, EBO_pod;
	
	glGenVertexArrays(1, &VAO_pod);
	glGenBuffers(1, &VBO_pod);
	glGenBuffers(1, &EBO_pod);

	glBindVertexArray(VAO_pod);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pod);
	glBufferData(GL_ARRAY_BUFFER, sizeof(podloga_cvorovi), podloga_cvorovi, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pod);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(podloga_ind), podloga_ind, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glEnable(GL_DEPTH_TEST);  
	
	bool lampa = true;
	float bar = 0.5, movement = 0.0, r = 4.0;
	
	glm::vec3 sijalica_pos(0.0f, 3.0f, 3.0f);
	
	while (!glfwWindowShouldClose(w)) {
		
		process_input(w);
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.4f, 0.35f, 0.35f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
		(float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();		
		glm::mat4 model = glm::mat4(1.0f);
		
		model_shader.use();
		model_shader.setMat4("projection", projection);
		model_shader.setMat4("view", view);
		
		model_shader.setBool("lampa", lampa);
		
		//model pointlight 
		model_shader.setVec3("pointLight.position", lightPos);
		model_shader.setVec3("viewPosition", camera.Position); 
		model_shader.setFloat("pointLight.constant",  1.0f);
		model_shader.setFloat("pointLight.linear",	  0.09f);
		model_shader.setFloat("pointLight.quadratic", 0.032f);		
		model_shader.setVec3("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		model_shader.setVec3("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		model_shader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		
		//model dirlight
		model_shader.setVec3("dirL.direction", glm::vec3(5.0f, 5.0f, 5.0f));
		model_shader.setVec3("dirL.ambient", glm::vec3(0.35f, 0.2f, 0.2f));
		model_shader.setVec3("dirL.diffuse", glm::vec3(0.1f, 0.1f, 0.3f));
		model_shader.setVec3("dirL.specular", 1.0f, 1.0f, 1.0f);
		
		//model spotlight
		model_shader.setVec3("spotLight.position", 
		glm::vec3(sijalica_pos.x, sijalica_pos.y + 0.5, sijalica_pos.z));
		model_shader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		model_shader.setVec3("spotLight.ambient", 0.4f, 0.3f, 0.5f);
		model_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		model_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		model_shader.setFloat("spotLight.constant", 1.0f);
		model_shader.setFloat("spotLight.linear", 0.09);
		model_shader.setFloat("spotLight.quadratic", 0.032);
		model_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(15.5f)));
		model_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(23.5f)));	   
		
		//sto
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model_shader.setMat4("model", model);
		
		table.Draw(model_shader);
		
		//sijalica
		model = glm::mat4(1.0f);
		model = glm::translate(model, sijalica_pos);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f,0.0f,0.0f));
		model_shader.setMat4("model", model);
		
		lightbulb.Draw(model_shader);
	
		//tet lights
		obj_shader.use();
		obj_shader.setBool("lampa", lampa);
		
		//light cube
		obj_shader.setVec3("light.position", lightPos);
		obj_shader.setVec3("viewPos", camera.Position); 
		obj_shader.setFloat("light.constant",  1.0f);
		obj_shader.setFloat("light.linear",	   0.09f);
		obj_shader.setFloat("light.quadratic", 0.032f);		
		obj_shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		obj_shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		obj_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//material
		obj_shader.setVec3("mat.ambient", 1.0f, 0.5f, 0.31f);
		obj_shader.setVec3("mat.diffuse", 1.0f, 0.5f, 0.31f);
		obj_shader.setVec3("mat.specular", 0.4f, 0.3f, 0.5f); 
		obj_shader.setFloat("mat.shininess", 32.0f);
		
		//spotlight
		obj_shader.setVec3("spotLight.position", sijalica_pos);
		obj_shader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		obj_shader.setVec3("spotLight.ambient", 0.4f, 0.3f, 0.5f);
		obj_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		obj_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		obj_shader.setFloat("spotLight.constant", 1.0f);
		obj_shader.setFloat("spotLight.linear", 0.09);
		obj_shader.setFloat("spotLight.quadratic", 0.032);
		obj_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		obj_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));	 
	   
		//dirlight
		obj_shader.setVec3("dirL.direction", glm::vec3(5.0f, 5.0f, 5.0f));
		obj_shader.setVec3("dirL.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		obj_shader.setVec3("dirL.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		obj_shader.setVec3("dirL.specular", 1.0f, 1.0f, 1.0f);
		
		//tet1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(glm::sin(currentFrame) - 0.1f, 1.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		obj_shader.setMat4("model", model);
		obj_shader.setMat4("projection", projection);
		obj_shader.setMat4("view", view);
		
		glBindVertexArray(VAO_tet);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		
		//tet2
		obj_shader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.8f + glm::sin(currentFrame), 1.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model,glm::radians(glm::sin(currentFrame) * 360.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));
		
		obj_shader.setMat4("model", model);
		obj_shader.setMat4("projection", projection);
		obj_shader.setMat4("view", view);
		
		obj_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		obj_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		obj_shader.setVec3("viewPos", camera.Position); 
	
		obj_shader.setVec3("mat.ambient", 0.0215, 0.1745, 0.0215);
		obj_shader.setVec3("mat.diffuse", 0.07568, 0.61424, 0.07568);
		obj_shader.setVec3("mat.specular", 0.633, 0.727811, 0.633); 
		obj_shader.setFloat("mat.shininess", 0.6 * 128.0);		
		
		glBindVertexArray(VAO_tet);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		
		//cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		
		light_shader.use();
		light_shader.setMat4("model", model);
		light_shader.setMat4("projection", projection);
		light_shader.setMat4("view", view);
		light_shader.setVec3("col", glm::vec3(1.0f, 0.2f, 0.4f));
		
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		//cube 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 3.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		
		light_shader.use();
		light_shader.setMat4("model", model);
		light_shader.setMat4("projection", projection);
		light_shader.setMat4("view", view);
		light_shader.setVec3("col", glm::vec3(1.0f, 0.2f, 0.4f));
		
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		//podloga
		
		podloga_shader.use();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		podloga_shader.setBool("lampa", lampa);
		
		podloga_shader.setVec3("pointLight.position", lightPos);
		podloga_shader.setVec3("viewPosition", camera.Position); 
		podloga_shader.setFloat("pointLight.constant",	1.0f);
		podloga_shader.setFloat("pointLight.linear",	0.09f);
		podloga_shader.setFloat("pointLight.quadratic", 0.032f);		
		podloga_shader.setVec3("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		podloga_shader.setVec3("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		podloga_shader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		
		//dirlight
		podloga_shader.setVec3("dirLight.direction", glm::vec3(5.0f, 5.0f, 5.0f));
		podloga_shader.setVec3("dirLight.ambient", glm::vec3(0.35f, 0.2f, 0.2f));
		podloga_shader.setVec3("dirLight.diffuse", glm::vec3(0.1f, 0.1f, 0.3f));
		podloga_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		
		//model spotlight
		podloga_shader.setVec3("spotLight.position", 
		glm::vec3(sijalica_pos.x, sijalica_pos.y + 0.5, sijalica_pos.z));
		podloga_shader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		podloga_shader.setVec3("spotLight.ambient", 0.4f, 0.3f, 0.5f);
		podloga_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		podloga_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		podloga_shader.setFloat("spotLight.constant", 1.0f);
		podloga_shader.setFloat("spotLight.linear", 0.09);
		podloga_shader.setFloat("spotLight.quadratic", 0.032);
		podloga_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(15.5f)));
		podloga_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(23.5f)));	 
		
		podloga_shader.setMat4("model", model);
		podloga_shader.setMat4("projection", projection);
		podloga_shader.setMat4("view", view);
		
		glBindTexture(GL_TEXTURE_2D, wojak_tex);
		glBindVertexArray(VAO_pod);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//svetlo 
		if(turn_on_off && glm::sin(currentFrame) * (bar > 0 ? 1 : -1) > bar) {
			lampa = !lampa;
			bar = -bar;
		}
		
		//krenja
		if(move_scene) {
			lightPos = glm::vec3(r * glm::sin(movement), r*glm::cos(movement), lightPos.z );
			sijalica_pos.x = glm::cos(movement);
			movement += 0.01;
		}
		
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
		
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		turn_on_off = !turn_on_off;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		move_scene = !move_scene;
}

int load_texture(std::string path) {
	
	stbi_set_flip_vertically_on_load(true);	 
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int tex_width, tex_height, nrChannels;
	
	unsigned char *data = stbi_load(path.c_str() ,&tex_width, &tex_height, &nrChannels, 0);
	
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, 
		tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load tex!\n";
	}
	stbi_image_free(data);
	
	return tex;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	if(key == GLFW_KEY_L && action == GLFW_PRESS)
		glEnable(GL_MULTISAMPLE);
	if(key == GLFW_KEY_K && action == GLFW_PRESS)
		glDisable(GL_MULTISAMPLE);
}