/*---------------------------------------------------------*/
/* ----------------   Proyecto final --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*------------- Alumno:Camargo Hernandez Alan ---------------*/
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include <windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib,"Winmm.lib")

#include "camera.h"
#include "Model.h"
#include "Texture.h"

// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint skyboxVBO, skyboxVAO;

//Camera
Camera camera(glm::vec3(2.5f, 1.0f, 5.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(7.0f, 4.0f, 3.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

void myData(void);
void myData2(void);
void display(Shader, Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//Texture
unsigned int	t_smile;

//Declaración de las variables para las animaciones
//Balon
float	ruedapelota = 0,
movpelotaX = 1.5,
movpelotay = 0.5;
//Coraje
float	
giraMano1 = 0,
giraMano2 = 0,
giraPiernas = 0,
girarTodoCoraje = -90,
movCorajeX = 1.0,
movCorajeZ = 1.4,
movCorajeY = 0.0;
float movpuerta = 0.0f;
float movHelicoteroX = 2.5,
movHelicoteroY=0.44,
movHelicoteroZ=-0.5,
rotTodoHeli = 0.0,
rothelice = 0.0;
float movautox = 1.4,
movautoy = 0,
movautoz = 0.9,
girarato = 90;

bool	animacionBalon = false,
recorridobalon1 = true,
recorridobalon2 = false,
recorridobalon3 = false,
recorridobalon4 = false,
recorridobalon5 = false,
recorridobalon6 = false,
rebotar = false;
bool	animacionCoraje = false,
recorridocoraje1 = true,
recorridocoraje2 = false,
recorridocoraje3 = false,
recorridocoraje4 = false,
recorridocoraje5 = false;

bool puerta = false,
abrirpuerta = true,
cerrarpuerta = false;

bool animacionHelicoptero = false,
recorridoHelicoptero1 = true,
recorridoHelicoptero2 = false,
recorridoHelicoptero3 = false,
recorridoHelicoptero4 = false,
recorridoHelicoptero5 = false,
recorridoHelicoptero6 = false,
recorridoHelicoptero7 = false,
recorridoHelicoptero8 = false,
recorridoHelicoptero9 = false;

bool animacionAuto = false,
recorridoauto1 = true,
recorridoauto2 = false,
recorridoauto3 = false,
recorridoauto4 = false,
recorridoauto5 = false,
recorridoauto6 = false;



//Keyframes
float	posX = -15.0f;
float	posY = 0.0f;
float	posZ = 0.0f;
float	rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	movBrazo = 0.0f;

float	incX,
		incY,
		incZ,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;
float	movBrazoInc = 0.0f;

#define MAX_FRAMES 9;
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX=5;		//Variable para PosicionX
	float posY=0;		//Variable para PosicionY
	float posZ=-5;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazo;

}FRAME;

FRAME KeyFrame[9];
int FrameIndex = 2;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

}

void resetElements(void)
{


}

void interpolation(void)
{



}


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void myData2()
{
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// SkyBox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

}


int porsilloCoraje = 0;
int cuenta = 0;
int decremento = 0.06f;

void animate(void)
{
	if (animacionBalon) {
		if (recorridobalon1) {
			ruedapelota += 3.0f;
			movpelotaX += 0.05f;
			if (movpelotaX >= 1.78f) {
				recorridobalon1 = false;
				recorridobalon2 = true;
			}
		}
		if (recorridobalon2) {
			movpelotay = movpelotay-0.04;
			if (movpelotay <= 0.126f) {
				recorridobalon2 = false;
				if (cuenta == 3) {
					recorridobalon6 = true;
					cuenta = 4;
					decremento = 0.03;
				}
				if (cuenta == 2) {
					recorridobalon5 = true;
					cuenta = 3;
					decremento = 0.03;
				}
				if (cuenta == 1) {
					recorridobalon4 = true;
					cuenta = 2;
				}
				if (cuenta == 0) {
					recorridobalon3 = true;
					cuenta = 1;
				}
			
				
			}
		}
		if (recorridobalon3) {
			movpelotay += 0.06f;
			if (movpelotay >= 0.7) {
				recorridobalon3 = false;
				recorridobalon2 = true;
			}
		}
		if (recorridobalon4) {
			movpelotay += 0.06f;
			if (movpelotay >= 0.5) {
				recorridobalon4 = false;
				recorridobalon2 = true;
			}
		}
		if (recorridobalon5) {
			movpelotay += 0.06f;
			if (movpelotay >= 0.3) {
				recorridobalon5 = false;
				recorridobalon2 = true;
			}
		}
		if (recorridobalon6) {
			movpelotay += 0.025f;
			if (movpelotay >= 0.15) {
				recorridobalon6 = false;
				recorridobalon2 = true;
				rebotar = true;
			}
		}

	}
	if (animacionCoraje) {
		if (porsilloCoraje == 0) {
			giraPiernas = 25;
			giraMano1 = 20;
			giraMano2 = -20;
			porsilloCoraje = 1;
		}
		else {
			giraMano1 = 0;
			giraMano2 = 0;
			giraPiernas = 0;
			porsilloCoraje = 0;
		}
		if (recorridocoraje1) {
			movCorajeX -= 0.03;
			girarTodoCoraje = -90;
			if (movCorajeX <= -1.7) {
				recorridocoraje1 = false;
				recorridocoraje2 = true;
			}
		}
		if (recorridocoraje2) {
			movCorajeZ -= 0.03;
			girarTodoCoraje = 180;
			if (movCorajeZ <= 0.7) {
				recorridocoraje2 = false;
				recorridocoraje3 = true;
			}
		}
		if (recorridocoraje3) {
			movCorajeX += 0.03;
			girarTodoCoraje = 90;
			if (movCorajeX >= -0.6) {
				recorridocoraje3 = false;
				recorridocoraje4 = true;
			}
		}
		if (recorridocoraje4) {
			movCorajeZ += 0.03;
			girarTodoCoraje = 0;
			if (movCorajeZ >= 1.4) {
				recorridocoraje4 = false;
				recorridocoraje5 = true;
			}
		}
		if (recorridocoraje5) {
			movCorajeX += 0.03;
			girarTodoCoraje = 90;
			if (movCorajeX >= 1) {
				recorridocoraje5 = false;
				recorridocoraje1 = true;
			}
		}	
	}
	
	
	if (puerta) {
		if (abrirpuerta) {
			movpuerta += 5.0f;
			if (movpuerta >= 90.0f) {
				abrirpuerta = false;
				puerta = false;
				cerrarpuerta = true;
			}
		}
		if (cerrarpuerta) {
			movpuerta -= 5.0f;
			if (movpuerta <= 0.0f) {
				cerrarpuerta = false;
				puerta = false;
				abrirpuerta = true;
			}
		}
	}
	if (animacionHelicoptero) {
		rothelice += 35.0;
		if (recorridoHelicoptero1) {
			movHelicoteroY += 0.03;
			rotTodoHeli = 0;
			if (movHelicoteroY >= 1.0) {
				recorridoHelicoptero1 = false;
				recorridoHelicoptero2 = true;
			}
		}
		if (recorridoHelicoptero2) {
			movHelicoteroZ += 0.03;
			rotTodoHeli = 0;
			if (movHelicoteroZ >= 1.8) {
				recorridoHelicoptero2 = false;
				recorridoHelicoptero3 = true;
			}
		}
		if (recorridoHelicoptero3) {
			movHelicoteroX-= 0.03;
			rotTodoHeli = -90;
			if (movHelicoteroX <= -1.6) {
				recorridoHelicoptero3 = false;
				recorridoHelicoptero4 = true;
			}
		}
		if (recorridoHelicoptero4) {
			movHelicoteroZ -= 0.03;
			rotTodoHeli = 180;
			if (movHelicoteroZ <= -1.0) {
				recorridoHelicoptero4 = false;
				recorridoHelicoptero5 = true;
			}
		}
		if (recorridoHelicoptero5) {
			movHelicoteroX += 0.03;
			rotTodoHeli = 90;
			if (movHelicoteroX >= -0.8) {
				recorridoHelicoptero5 = false;
				recorridoHelicoptero6 = true;
			}
		}
		if (recorridoHelicoptero6) {
			movHelicoteroZ += 0.03;
			rotTodoHeli = 0;
			if (movHelicoteroZ >= 1.8) {
				recorridoHelicoptero6 = false;
				recorridoHelicoptero7 = true;
			}
		}
		if (recorridoHelicoptero7) {
			movHelicoteroX += 0.03;
			rotTodoHeli = 90;
			if (movHelicoteroX >= 1.4) {
				recorridoHelicoptero7 = false;
				recorridoHelicoptero8 = true;
			}
		}
		if (recorridoHelicoptero8) {
			movHelicoteroZ -= 0.03;
			rotTodoHeli = 180;
			if (movHelicoteroZ <= -0.5) {
				recorridoHelicoptero8 = false;
				recorridoHelicoptero9 = true;
			}
		}
		if (recorridoHelicoptero9) {
			movHelicoteroX += 0.03;
			rotTodoHeli = 90;
			if (movHelicoteroX >= 2.5) {
				recorridoHelicoptero9 = false;
				recorridoHelicoptero2 = true;
			}
		}
	}
	
	if (animacionAuto) {
		if (recorridoauto1) {
			movautox -= 0.04;
			girarato = 90;
			if (movautox <= 0.9) {
				recorridoauto1 = false;
				recorridoauto2 = true;
			}
		}
		if (recorridoauto2) {
			movautoz -= 0.04;
			girarato = 0;
			if (movautoz <= -1.2) {
				recorridoauto2 = false;
				recorridoauto3 = true;
			}
		}
		if (recorridoauto3) {
			movautox += 0.04;
			girarato = -90;
			if (movautox >= 2.2) {
				recorridoauto3 = false;
				recorridoauto4 = true;
			}
		}
		if (recorridoauto4) {
			movautoz += 0.04;
			girarato = 180;
			if (movautoz >= 0.9) {
				recorridoauto4 = false;
				recorridoauto1 = true;
			}
		}
		
	}
	/*
		if (recorridocoraje2) {

			if (movpelotay <= 0.15f) {
				recorridocoraje2 = false;
				recorridocoraje1 = true;
			}
		}
		if (recorridocoraje3) {
			movpelotay += 0.04f;
			if (movpelotay >= indicador) {
				recorridocoraje3 = false;
				recorridocoraje4 = false;
			}
		}
	}*/
	
}
void sound() {
	printf("Now playing: cancion");
	//mciSendString("open \"\cancion.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	//mciSendString("play mp3", NULL, 0, NULL);
}

void display(Shader shader, Shader skyboxShader, GLuint skybox,
	Model casa,
	Model cama, Model mesa,
	Model sillon, Model silla,
	Model box, Model buro,
	Model tv, Model maceta,
	Model racktv, Model cuadro,
	Model puerta, Model cuadro2,
	Model sofa, Model marco,
	Model balon, Model cascovader,
	Model bIzCoraje, Model corajecuerpo,
	Model corajepiernas, Model Helicoptero,
	Model Helice, Model bderCoraje,
	Model autom
)
{
	shader.use();

	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.7, 0.7, 0.7));
	shader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.4f));
	shader.setVec3("dirLight.specular", glm::vec3(0.7, 0.7, 0.7));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[0].constant", 1.0f);
	shader.setFloat("pointLight[0].linear", 0.009f);
	shader.setFloat("pointLight[0].quadratic", 0.0032f);

	shader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.009f);
	shader.setFloat("pointLight[1].quadratic", 0.032f);

	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 coraje = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);



	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	casa.Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.76f, 0.0f, 2.5f));
	model = glm::rotate(model, glm::radians(movpuerta), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.008f, 0.0081f, 0.00825f));
	shader.setMat4("model", model);
	puerta.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.3f, 0.0f, 2.5f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.023f, 0.023f));
	shader.setMat4("model", model);
	marco.Draw(shader);

	//cuarto 1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.0275f, -1.5f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	cama.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.7f, 0.0f, -2.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	buro.Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, 0.0f, -2.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	buro.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.8f, -2.10f));
	model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
	shader.setMat4("model", model);
	cuadro.Draw(shader);

	//cuarto 2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.8f, -2.10f));
	model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
	shader.setMat4("model", model);
	cuadro2.Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.7f, 0.0f, -0.5f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	model = glm::translate(model, glm::vec3(0.0f, 43.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	maceta.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -2.0f));
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	shader.setMat4("model", model);
	box.Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	shader.setMat4("model", model);
	racktv.Draw(shader);
	model = glm::translate(model, glm::vec3(0.0f, 71.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	tv.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f, 0.0f, -1.7f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	silla.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.8f, 0.0f, -1.7f));
	model = glm::scale(model, glm::vec3(0.009f, 0.009f, 0.009f));
	shader.setMat4("model", model);
	sofa.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.8f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	shader.setMat4("model", model);
	sillon.Draw(shader);
	//animcaciones
	model = glm::translate(glm::mat4(1.0f), glm::vec3(movpelotaX, movpelotay, -2.0f));
	model = glm::rotate(model, glm::radians(ruedapelota), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	balon.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movCorajeX, movCorajeY, movCorajeZ));
	model = glm::rotate(model, glm::radians(girarTodoCoraje), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	corajecuerpo.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movCorajeX, movCorajeY+0.3f, movCorajeZ));
	model = glm::rotate(model, glm::radians(girarTodoCoraje), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(giraMano1), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	bIzCoraje.Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(movCorajeX, movCorajeY+0.3f, movCorajeZ));
	model = glm::rotate(model, glm::radians(girarTodoCoraje), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(giraMano2), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	bderCoraje.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movCorajeX, movCorajeY, movCorajeZ));
	model = glm::rotate(model, glm::radians(girarTodoCoraje), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(giraPiernas), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	corajepiernas.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movHelicoteroX, movHelicoteroY, movHelicoteroZ));
	model = glm::rotate(model, glm::radians(rotTodoHeli), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	shader.setMat4("model", model);
	Helicoptero.Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(movHelicoteroX, movHelicoteroY+0.11, movHelicoteroZ));
	model = glm::rotate(model, glm::radians(rothelice), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	shader.setMat4("model", model);
	Helice.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movautox, movautoy, movautoz));
	model = glm::rotate(model, glm::radians(girarato), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	shader.setMat4("model", model);
	autom.Draw(shader);

	

	
	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Camargo Hernández Alan M(no me repruebe profe)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, my_input);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData2();
	glEnable(GL_DEPTH_TEST);
	
	//Shaders
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	// Load models
	Model casaModel = ((char *)"Models/Casa/casa.obj"); 
	Model puertaModel = ((char *)"Models/Casa/sspuerta.obj");
	Model marcoModel = ((char *)"Models/Casa/marco.obj");
	

	Model tableModel = ((char *)"Models/table/mesacentro.obj");
	Model camaModel = ((char *)"Models/cama/cama.obj");
	Model sillonModel = ((char *)"Models/chairarm/sillon.obj");
	Model sillaModel = ((char *)"Models/chairarm/silla.obj");

	Model boxModel = ((char *)"Models/chairarm/box.obj");
	Model buroModel = ((char *)"Models/cama/buro.obj");
	Model tvModel = ((char *)"Models/tv/tv.obj");
	Model macetaModel = ((char *)"Models/flopot/meceta.obj");
	Model racktvModel = ((char *)"Models/tv/ptv.obj");
	Model cuadroModel = ((char *)"Models/cuadro/cuadro.obj");
	Model cuadro2Model = ((char *)"Models/cuadro2/cuadro.obj");
	Model sofaModel = ((char *)"Models/chairarm/sofa.obj"); 
	Model balonModel = ((char *)"Models/bBASKET/ball.obj");
	Model vadercascoModel = ((char *)"Models/vader/casco.obj");

	Model corajeBrazosModel = ((char *)"Models/coraje/biz.obj");
	Model corajeBrazoderModel = ((char *)"Models/coraje/bde.obj");
	Model corajeCuerpoModel = ((char *)"Models/coraje/cuerpo.obj");
	Model corajepiernasModel = ((char *)"Models/coraje/piernas.obj");
	
	Model HelicopModel = ((char *)"Models/Heli/heli.obj");
	Model helicModel = ((char *)"Models/Heli/helic.obj");

	Model autoModel = ((char *)"Models/Carro1/Carro1.obj");
	//Inicialización de KeyFrames
	for (int i = 0; i < 9; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
    // While the windows is not closed
	sound();
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        //my_input(window);
		animate();

        // render
        // Background color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(modelShader, SkyBoxshader, cubemapTexture, 
			casaModel, 
			camaModel, tableModel,
			sillonModel, sillaModel, 
			boxModel,buroModel,
			tvModel,macetaModel,
			racktvModel, cuadroModel,
			puertaModel, cuadro2Model,
			sofaModel, marcoModel,
			balonModel, vadercascoModel,
			corajeBrazosModel, corajeCuerpoModel,
			corajepiernasModel, HelicopModel,
			helicModel, corajeBrazoderModel,
			autoModel
		);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		animacionBalon ^= true;
		if (rebotar) {
			recorridobalon3 = true;
			cuenta = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		animacionCoraje ^= true;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		animacionHelicoptero ^= true;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		animacionAuto ^= true;
	
	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		puerta ^= true;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}