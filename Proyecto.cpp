/*---------------------------------------------------------*/
/* ----------------  Proyecto            -----------*/
/*-----------------    2022-2   ---------------------------*/
/*------------- Alumno:Montiel Moreno Oscar  ---------------*/
/*------------- No. Cuenta   315230530  -------------------*/
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);
 
// camera
Camera camera(glm::vec3(0.0f, 200.0f, 800.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);	//En esta parte la posición de donde viene la fuente de luz direccional
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);	//Aquí configuro desde dónde viene la luz

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		avanza = true;		//Para avanzar el vehículo en el ciclo

float giroLlantas = 0.0;
float rot200 = 0.0f;	//para rotar el vehículo

float	miVariable, miVar = 0.0f;	//declaro variable
float Crojo, Cverde, Cazul = 0.0f;

//*************       MODIFICACIONES Proyecto     ****************************
float   escalaAuto1_JCA = 0.3f;
float	escalaEdificio1_JCA = 4.0f;
float	escalaEdificio2_JCA = 5.0f;
float	escalaEntrada_JCA = 1.0f;// Escala	 y			Escala			 x
								//  0.8		433			0.1				-96
								//  0.1		 ?			nueva escala	 ?


//animaciones

bool	animaTren = false, 
		animaCarro = false,
		animaBronto = false,
		animaAtaque = false,
		animaMeteor = false;

//*************************************************************************************************************

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)	//aquí hago que se vuelva automatica y no tenga que moverla luz
{
	//Crojo += 0.005f;
	//Cverde += 0.0001;
	//Cazul += 0.001f;



	lightPosition.x = 500.0f * cos(miVariable);	//manipulo su posición en eje horizontal
	lightPosition.y = 600.0f * sin(miVariable);//manipulo profundidad z// cambio a altura con y

	miVariable -= 0.009f;

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//xxxxxxxxxxxxxxxxxxxxxxxxx	ANIMACIONES PROYECTO	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//




	//**************************		ANIMATREN		*******************************//
	if (animaTren)
	{

	}
	//__________________________	FIN	ANIMATREN		___________________________________//


	//**************************		ANIMACARRO		*******************************//
	if (animaCarro)
	{

	}
	//__________________________	FIN	ANIMACARRO		___________________________________//

	//**************************		ANIMABRONTO		*******************************//
	if (animaBronto)
	{

	}
	//__________________________	FIN	ANIMABRONTO		___________________________________//

		//**************************		ANIMAATAQUE		*******************************//
	if (animaAtaque)
	{

	}
	//__________________________	FIN	ANIMAATAQUE		___________________________________//

		//**************************		ANIMAMETEOR		*******************************//
	if (animaMeteor)
	{

	}
	//__________________________	FIN	ANIMAMETEOR		___________________________________//


	//xxxxxxxxxxxxxxxxxxxxxxxxx+++++++++++	FIN DE ANIMACIONES PROYECTO	+++++++++++++++xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

	//Vehículo
	if (animacion)
	{
		if (avanza)
		{
			movAuto_z += 2.3f;
			giroLlantas += 3.0f;
			//if (movAuto_z == 200.0f)
				
			if (movAuto_z >= 200.0f)
			{
				avanza = false;
				rot200 = rot200 + 180.0f;
			}
		}
		else
		{
			movAuto_z -= 2.3f;
			giroLlantas -= 3.0f;
			if (movAuto_z <= 0.0f)
			{
				avanza = true;
				rot200 = rot200 + 180.0f;
			}
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");//shaders
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//Es el fondo Recursos y Skybox, es un cubo donde se incluyen las 6 texturas y es estatico (se puede usar skydom)
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");//parte de documentación

	vector<std::string> faces
	{
		"resources/skybox/Proyecto/derecha.jpg",	//derecha
		"resources/skybox/Proyecto/izquierda.jpg",	//izquierda
		"resources/skybox/Proyecto/arriba.jpg",		//arriba
		"resources/skybox/Proyecto/abajo.jpg",		//fondo
		"resources/skybox/Proyecto/frente.jpg",		//frente
		"resources/skybox/Proyecto/atras.jpg"		//atrás
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	//a partir de aquí se cargan los modelos
	// load models
	// -----------

	Model piso("resources/objects/piso/piso.obj");	//para estaticos sólo Model


	//ModelAnim dummy("resources/objects/Dummy/HipHopDancing.dae");	//agrego al Velociraptor de hueso
	//dummy.initShaders(animShader.ID);

	// ANIMACIONES PROYECTO

	//Coche easter egg
	Model cuboAuto1("resources/objects/CuboAuto1/cuboAuto1.obj");
	Model cuboAuto1Vidrio("resources/objects/CuboAuto1/cuboAuto1-vidrio.obj");
	Model llantaAuto1("resources/objects/CuboAuto1/llantaAuto1.obj");

	//AUTO2_JCA
	Model Auto2("resources/objects/Auto2JCA/Auto2_JCA.obj");
	Model LlantaAuto2("resources/objects/Auto2JCA/LlantaAuto2_JCA.obj");

	//Edificios
	Model Edificio1("resources/objects/Edificio1/Edificio1.obj");
	Model Edificio2("resources/objects/Edificio2/Edificio2.obj");
	Model Edificio3("resources/objects/Edificio3/Edificio3.obj");
	Model Edificio4("resources/objects/Edificio4/Edificio4_JCA.obj"); 
	Model Marco("resources/objects/Entrada/Marco.obj");
	Model PuertaDer("resources/objects/Entrada/PuertaDer.obj");
	Model PuertaIzq("resources/objects/Entrada/PuertaIzq.obj");
	Model Hotel("resources/objects/Hotel/hotel.obj");

	//Naturaleza y dinosaurios
	Model Arboles("resources/objects/Bosque/trees/arboles.obj");
	Model ArbolesLinea("resources/objects/Bosque/trees/arbolesLinea.obj");
	Model Quetzal("resources/objects/Dinos/Quetzal/13623_Quetzalcoatlus_v1_L2.obj"); 

	//Helicoptero
	Model Helicoptero("resources/objects/Helicoptero/Helicoptero.obj");

	Model VelociBone("resources/objects/VelociraptorBone/VelociBone.obj");
	//BRONTOSAURIO
	Model Bronto("resources/objects/Brontosaurio/Brontosaurio1.obj");
	Model CabezaBronto("resources/objects/Brontosaurio/cabeza.obj");
	Model TorsoBronto("resources/objects/Brontosaurio/torso.obj");
	Model PiernaTIBronto("resources/objects/Brontosaurio/piernaTI.obj");
	Model PiernaTDBronto("resources/objects/Brontosaurio/piernaTD.obj");
	Model PiernaDIBronto("resources/objects/Brontosaurio/piernaDI.obj");
	Model PiernaDDBronto("resources/objects/Brontosaurio/piernaDD.obj");
	Model CuelloBronto("resources/objects/Brontosaurio/cuello.obj");
	Model ColaBronto("resources/objects/Brontosaurio/cola.obj");

	//TREX
	Model Trex("resources/objects/Trex/Trex2.obj");


	//TREN
	Model Riel("resources/objects/Vias/riel1.obj");
	Model Tren("resources/objects/Tren/Tren.obj");
	Model Vagon1("resources/objects/Tren/Vagon1_1.obj");
	Model Vagon2("resources/objects/Tren/Vagon2_1.obj");

	//METEORITO
	Model Meteor("resources/objects/Meteorito/Meteorito.obj");

	ModelAnim Meteorito("resources/objects/Meteorito/Asteroide.dae");	//agrego al meteorito
	Meteorito.initShaders(animShader.ID);

	//ModelAnim lowDino("resources/objects/LowDino/LowDino.dae");	//agrego al dino
	//lowDino.initShaders(animShader.ID);

	 //comentado por el momento en esta sesión 24/03/22
	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");// Para animaciones es ModelAnim
	//animacionPersonaje.initShaders(animShader.ID);

	//ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	//ninja.initShaders(animShader.ID);
	////////////////

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights					//a continuacipon una fuente direccional y dos fuentes posicionales
		staticShader.setVec3("viewPos", camera.Position);			// fuente de luz direccional
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));	//aquí prendo y apago luz direccionañ
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));	//
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));	//

		//staticShader.setVec3("pointLight[0].position", lightPosition);		//fuente de luz posicional 1
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.6f, 0.6f, 0.6f));
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setFloat("pointLight[0].constant", 0.0008f);	//potencia de los rayos luminosos (fino de potencia)
		//staticShader.setFloat("pointLight[0].linear", 0.0009f);	//es más brusco que el cte
		//staticShader.setFloat("pointLight[0].quadratic", 0.00000000000000000032f);	//distancia que pueden recorrer los rayos luminosos
		
		staticShader.setVec3("pointLight[0].position", glm::vec3(0.0f, 0.0f, 0.0f));	//fuente de luz posicional 2
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		//staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));	//fuente de luz posicional 2
		//staticShader.setVec3("pointLight[1].ambient", glm::vec3(Crojo, Cazul, Cverde));
		//staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setFloat("pointLight[1].constant", 1.0f);
		//staticShader.setFloat("pointLight[1].linear", 0.009f);
		//staticShader.setFloat("pointLight[1].quadratic", 0.00032f);


		//staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 4.0f, 20.0f));	//fuente de luz posicional 3
		//staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.6f));		//al costado la vuelve azul
		//staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.7f, 0.0f, 0.6f));		//luz centro anteriormente amarilla-ahora magenta
		//staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setFloat("pointLight[2].constant", 1.0f);
		//staticShader.setFloat("pointLight[2].linear", 0.009f);
		//staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmpCA1 = glm::mat4(1.0f);//Temporal para cubo auto 1
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);



		// -------------------------------------------------------------------------------------------------------------------------
		// Animaciones Proyecto
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, -50.0f)); // agrego al dinosaurio Low Dino
		model = glm::scale(model, glm::vec3(1.0f));	//
		animShader.setMat4("model", model);
		//Meteorito.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Fin Animaciones Proyecto
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		// -------------------------------------------------------------------------------------------------------------------------
		// Modelos Proyecto
		// -------------------------------------------------------------------------------------------------------------------------
	
		//---------------------------------------------VELOCIBONE---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-850.0f, 0.0f, 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		VelociBone.Draw(staticShader);

		//---------------------------------------------FIN VELOCIBONE---------------------------------------------------//

		
		//--------------------------------------------- BRONTOSAURIO---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2200.0f, 0.0f, 1500.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f));
		staticShader.setMat4("model", model);
		Bronto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2200.0f, 0.0f, 1500.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		//TorsoBronto.Draw(staticShader);

		//--------------------------------------------- FIN	BRONTOSAURIO---------------------------------------------------//

		//--------------------------------------------- TREX---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, 2700.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		Trex.Draw(staticShader);

		//--------------------------------------------- FIN	TREX---------------------------------------------------//


		//--------------------------------------------- TREN---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 45.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Riel.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 45.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		//Tren.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 45.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Vagon1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 45.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Vagon2.Draw(staticShader);
		//--------------------------------------------- FIN DE TREN---------------------------------------------------//

		//--------------------------------------------- EDIFICIOS---------------------------------------------------//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2150.0f, 0.0f,2150.0f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0));
		staticShader.setMat4("model", model);
		Hotel.Draw(staticShader);	//Edificio Hotel izquierdo

		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEdificio1_JCA));
		staticShader.setMat4("model", model);
		Edificio1.Draw(staticShader);	//Edificio de entrada

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2000.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEdificio2_JCA));
		staticShader.setMat4("model", model);
		Edificio2.Draw(staticShader);	//Kiosco derecho

		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, 10.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Edificio3.Draw(staticShader);	//Edificio principal de tren
		//glEnable(GL_BLEND);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 0.0f, 800.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0));
		staticShader.setMat4("model", model);
		Edificio4.Draw(staticShader);	//Edificio Cerca de hotel


		model = glm::translate(glm::mat4(1.0f), glm::vec3(930.0f, 0.0f, -615.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEntrada_JCA));
		staticShader.setMat4("model", model);
		Marco.Draw(staticShader);	//Marco de las puertas

		model = glm::translate(glm::mat4(1.0f), glm::vec3(930.0f, 0.0f, -615.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEntrada_JCA));
		staticShader.setMat4("model", model);
		PuertaDer.Draw(staticShader);	//Puerta Derecha

		model = glm::translate(glm::mat4(1.0f), glm::vec3(930.0f, 0.0f, -615.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEntrada_JCA));
		staticShader.setMat4("model", model);
		PuertaIzq.Draw(staticShader);	//Puerta izquierda


		//--------------------------------------------- FIN DE EDIFICIOS---------------------------------------------------//

		//--------------------------------------------- NATURALEZA Y DINOSAURIOS---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1400.0f, 0.0f, 1480.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Arboles.Draw(staticShader);	//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1730.0f, 0.0f, 2180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		ArbolesLinea.Draw(staticShader);	//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 80.0f, 1180.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		Quetzal.Draw(staticShader);	//Quetzalcoatlus

		//--------------------------------------------- FIN DE NATURALEZA Y DINOSAURIOS---------------------------------------------------//


		//--------------------------------------------- HELICOPTERO---------------------------------------------------//



		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 300.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		Helicoptero.Draw(staticShader);	//Edificio principal de tren
		glEnable(GL_BLEND);

		//--------------------------------------------- FIN DE HELICOPTERO---------------------------------------------------//
		//

		//*************       MODIFICACIONES DE Auto   ****************************
		// 
		//Estas líneas son para habilitar la transparencia de las texturas
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Cubo para el primer auto: cuboAuto1
		//Se realizó el cubo con 3dmax además de agregarle una textura. Inicialmente mide 40 por lado
		//tmpCA1 = model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, 30.0f));
		tmpCA1 = model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, -1.0f, 30.0f));
		model = glm::translate(tmpCA1, glm::vec3(0.0f * escalaAuto1_JCA, 2.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.55f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA, 0.3f * escalaAuto1_JCA));//Con esto medirá x = 22, y = 10, z = 12
		staticShader.setMat4("model", model);
		cuboAuto1.Draw(staticShader);

		tmpCA1 = model = glm::translate(tmpCA1, glm::vec3(3.0f * escalaAuto1_JCA, 12.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.3f * escalaAuto1_JCA, 0.3f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 12, y = 12, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1.Draw(staticShader);

		/*//Intento de poner parte de enfrente del coche más completa
		model = glm::translate(tmpCA1, glm::vec3(-10.0f, 0.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-18.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.0125f, 0.3f, 0.05f));//x = 0.5, y = 10, z = 0.5
		staticShader.setMat4("model", model);
		cuboAuto1.Draw(staticShader);
		*/

		//Ventana frontal para el auto 1 (cuboAuto1)
		model = glm::translate(tmpCA1, glm::vec3(-8.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.1f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 4, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-7.75f * escalaAuto1_JCA, 1.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.0875f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 3.5, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-7.5f * escalaAuto1_JCA, 2.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.075f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 3, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-7.25f * escalaAuto1_JCA, 3.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.0625f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 2.5, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-7.0f * escalaAuto1_JCA, 4.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.05f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 2.0, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-6.75f * escalaAuto1_JCA, 5.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.0375f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 1.5, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-6.5f * escalaAuto1_JCA, 6.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.025f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 1.0, y = 1, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-6.25f * escalaAuto1_JCA, 7.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.0125f * escalaAuto1_JCA, 0.05f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 0.5, y = 2, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		model = glm::translate(tmpCA1, glm::vec3(-6.125f * escalaAuto1_JCA, 9.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.00625f * escalaAuto1_JCA, 0.05f * escalaAuto1_JCA, 0.25f * escalaAuto1_JCA));//x = 0.25, y = 2, z = 10
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		//Ventana lateral 1 para auto 1 (cuboAuto1)
		model = glm::translate(tmpCA1, glm::vec3(0.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA, 5.5f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.25f * escalaAuto1_JCA, 0.275f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA));//x = 10, y = 11, z = 1
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		//Ventana lateral 2 para auto 1 (cuboAuto1)
		model = glm::translate(tmpCA1, glm::vec3(0.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA, -5.5f * escalaAuto1_JCA));
		model = glm::scale(model, glm::vec3(0.25f * escalaAuto1_JCA, 0.275f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA));//x = 10, y = 11, z = 1
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		//Ventana trasero para auto 1 (cuboAuto1)
		model = glm::translate(tmpCA1, glm::vec3(6.5f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA, 0.0f * escalaAuto1_JCA));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f * escalaAuto1_JCA, 0.275f * escalaAuto1_JCA, 0.025f * escalaAuto1_JCA));//x = 10, y = 11, z = 1
		staticShader.setMat4("model", model);
		cuboAuto1Vidrio.Draw(staticShader);

		//Llanta trasera izquierda para auto 1 (cuboAuto1). Originalmente tiene un radio de 20 y una altura de 10
		model = glm::translate(tmpCA1, glm::vec3(2.0f * escalaAuto1_JCA, -11.0f * escalaAuto1_JCA, 2.0f * escalaAuto1_JCA));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f * escalaAuto1_JCA, 0.2f * escalaAuto1_JCA, 0.125f * escalaAuto1_JCA));//x = 5, y = 2, z = 5
		staticShader.setMat4("model", model);
		llantaAuto1.Draw(staticShader);

		//Llanta trasera derecha para auto 1 (cuboAuto1). Originalmente tiene un radio de 20 y una altura de 10
		model = glm::translate(tmpCA1, glm::vec3(2.0f * escalaAuto1_JCA, -11.0f * escalaAuto1_JCA, -4.0f * escalaAuto1_JCA));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f * escalaAuto1_JCA, 0.2f * escalaAuto1_JCA, 0.125f * escalaAuto1_JCA));//x = 5, y = 2, z = 5
		staticShader.setMat4("model", model);
		llantaAuto1.Draw(staticShader);

		//Llanta delantera izquierda para auto 1 (cuboAuto1). Originalmente tiene un radio de 20 y una altura de 10
		model = glm::translate(tmpCA1, glm::vec3(-8.0f * escalaAuto1_JCA, -11.0f * escalaAuto1_JCA, 2.0f * escalaAuto1_JCA));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f * escalaAuto1_JCA, 0.2f * escalaAuto1_JCA, 0.125f * escalaAuto1_JCA));//x = 5, y = 2, z = 5
		staticShader.setMat4("model", model);
		llantaAuto1.Draw(staticShader);

		//Llanta delantera derecha para auto 1 (cuboAuto1). Originalmente tiene un radio de 20 y una altura de 10
		model = glm::translate(tmpCA1, glm::vec3(-8.0f * escalaAuto1_JCA, -11.0f * escalaAuto1_JCA, -4.0f * escalaAuto1_JCA));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.125f * escalaAuto1_JCA, 0.2f * escalaAuto1_JCA, 0.125f * escalaAuto1_JCA));//x = 5, y = 2, z = 5
		staticShader.setMat4("model", model);
		llantaAuto1.Draw(staticShader);



		//*************************************************************************************************************


		//--------------------------------------------- TREX---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 400.0f, 100.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Meteor.Draw(staticShader);

		//--------------------------------------------- FIN	TREX---------------------------------------------------//



		// -------------------------------------------------------------------------------------------------------------------------
		// Fin Modelos Proyecto
		// -------------------------------------------------------------------------------------------------------------------------



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	//animación cambia de falso a verdadero y así
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
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightPosition.y++;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		lightPosition.y--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion ^= true;
	}

	//Teclas animaciones Proyecto
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animaTren ^= true;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		animaCarro^= true;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		animaBronto ^= true;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		animaAtaque ^= true;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		animaMeteor ^= true;
	}


	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
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

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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