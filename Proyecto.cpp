/*---------------------------------------------------------*/
/* ----------------  Proyecto            -----------*/
/*-----------------    2022-2   ---------------------------*/
/*------------- Alumno: Montiel Moreno Oscar (grupo 1) ---------------*/
/*------------- Alumno: Castro Alonso Jéssica (grupo 6)---------------*/
/*------------- Alumno: López Hernández Emmanuel (grupo 1)---------------*/
/*-----------------	 Equipo:7    -------------------*/



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
rotaCabeza = 0.0f,
rotaCola = 0.0f,
OrientaCuerpoPez = 0.0f,
OrientaColaPez = 0.0f,
PosIniPez_x = 600.0f,
PosIniPez_z = 600.0f,
PosPez_x = 0.0f,
PosPez_z = 0.0f,
MiVariablePez = 0.0f,
		orienta = 0.0f;

bool	animacion = false,
		MueveColaIzq = false,
		MueveColaDer = true,
		BajaCabeza = true,
		SubeCabeza = false,
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
float   escalaAuto1_JCA = 3.0f;
float	escalaEdificio1_JCA = 4.0f;
float	escalaEdificio2_JCA = 5.0f;
float	escalaEntrada_JCA = 1.0f;// Escala	 y			Escala			 x
								//  0.8		433			0.1				-96
								//  0.1		 ?			nueva escala	 ?



//*******************//Variables Para Tren **********************************

float movTren_x = 150.0f,
movTren_y = 0.0f,
movTren_z = 520.0f,
movTren_xV3 = 150.0f,//+101
movTren_zV3 = 621.0f,
rotaV1 = 0.0f,
rotaV2 = 0.0f,
rotaV3 = 0.0f;

bool AvanzaZ = true,
AvanzaZN = false,
AvanzaX = false,
GiraIzquierdaV1 = true,
GiraDerechaV1 = false,
GiraIzquierdaV2 = true,
GiraDerechaV2 = false,
GiraIzquierdaV3 = true,
GiraDerechaV3 = false;




//******************* FIN Variables Para Tren **********************************

//*******************//Variables Para Auto 2**********************************
float	escalaAuto2_JCA = 1.0f;
float	escalaLlantaAuto2_JCA = 2.0f;



//Posición inicial del auto 2
float	posAuto2_x = 930.0f,
posAuto2_y = 8.0f,
posAuto2_z = -900.0f;

//Para la translación del auto 2
float	movAuto2_x = 0.0f,
movAuto2_y = 0.0f,
movAuto2_z = 0.0f,
orientaAuto2 = 90.0f,    //Orientación que de modifica durante el recorrido
orientaInicial2 = -90.0f, //Orientación inicial del auto 2
orienta2 = 90.0f;        //Orientación de llantas

//Recorido del auto 2
bool	recorrido0_A2 = true,
recorrido0_giro_A2 = false,
recorrido1_A2 = false,
recorrido1_giro_A2 = false,
recorrido2_A2 = false,
recorrido3_A2 = false,
recorrido4_A2 = false;

//******************* FIN Variables Para Auto 2**********************************
//animaciones

bool	animaTren = false, 
		animaCarro = false,
		animaBronto = false,
		animaAtaque = false,
		animaMeteor = false,
		MovRaptor = false,
		MovPez = false;

//*************************************************************************************************************


//**********************Brontosaurio toma agua***********************************
float	escalaBrontosaurio_JCA = 70.0f;
float	orientaInicialBrontosaurio_JCA = 0.0f;
float	posX_Bronto = 1600.0f,
posY_Bronto = 192.5f,
posZ_Bronto = 0.0f;

//Keyframes (Manipulación y dibujo)
float	posX_Bronto_K = 0.0f,
posY_Bronto_K = 0.0f,
posZ_Bronto_K = 0.0f,
rotBrontoCabeza = 0.0f,
rotBrontoCuello = 0.0f,
rotBrontoCola = 0.0f,
rotBrontoPiernaDD = 0.0f,
rotBrontoPiernaDI = 0.0f,
rotBrontoPiernaTD = 0.0f,
rotBrontoPiernaTI = 0.0f;
float	incX_Bronto_K = 0.0f,
incY_Bronto_K = 0.0f,
incZ_Bronto_K = 0.0f,
incRotBrontoCabeza = 0.0f,
incRotBrontoCuello = 0.0f,
incRotBrontoCola = 0.0f,
incRotBrontoPiernaDD = 0.0f,
incRotBrontoPiernaDI = 0.0f,
incRotBrontoPiernaTD = 0.0f,
incRotBrontoPiernaTI = 0.0f;

//*************************************************************************************************************





//********************Para persona aterrada**********************
bool	animacion2_JCA = false;//La utilizo para animación de la persona asustada

float	escalaPersonaAterrada_JCA = 0.2f;
float	orientaInicialPersonaAterrada_JCA = 0.0f;
float	posPersonaAterrada_JCA_x = 1000.0f,
posPersonaAterrada_JCA_y = 0.0f,
posPersonaAterrada_JCA_z = 900.0f;

float	movPersonaAterrada_JCA_x = 0.0f,
movPersonaAterrada_JCA_y = 0.0f,
movPersonaAterrada_JCA_z = 0.0f,
orientaPersonaAterrada_JCA = 0.0f;

float	miVariable1_JCA = 0.0f;

bool	recorrido0_PA_JCA = true,
recorrido1_PA_JCA = false,
recorrido2_PA_JCA = false;


float	escalaQuetzal_JCA = 0.4f;
float	orientaCentroQuetzal_JCA_x = 0.0f,
orientaCentroQuetzal_JCA_y = -90.0f,
orientaAla1Quetzal_JCA_y = 0.0f,
orientaAla2Quetzal_JCA_y = 0.0f;
float	posQuetzal_JCA_x = posPersonaAterrada_JCA_x - 100.0f,
posQuetzal_JCA_y = 70.0f,
posQuetzal_JCA_z = posPersonaAterrada_JCA_z;

//*************************** FIN persona aterrada  ********************************************************************


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

#define MAX_FRAMES 15
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

	/*******************************MODIFICACIONES DE JESS*******************************/
	float	posX_Bronto_K = 0.0f,
		posY_Bronto_K = 0.0f,
		posZ_Bronto_K = 0.0f,
		rotBrontoCabeza = 0.0f,
		rotBrontoCuello = 0.0f,
		rotBrontoCola = 0.0f,
		rotBrontoPiernaDD = 0.0f,
		rotBrontoPiernaDI = 0.0f,
		rotBrontoPiernaTD = 0.0f,
		rotBrontoPiernaTI = 0.0f;
	/************************************************************************************/


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


	KeyFrame[FrameIndex].posX_Bronto_K = posX_Bronto_K;
	KeyFrame[FrameIndex].posY_Bronto_K = posY_Bronto_K;
	KeyFrame[FrameIndex].posZ_Bronto_K = posZ_Bronto_K;
	KeyFrame[FrameIndex].rotBrontoCabeza = rotBrontoCabeza;
	KeyFrame[FrameIndex].rotBrontoCuello = rotBrontoCuello;
	KeyFrame[FrameIndex].rotBrontoCola = rotBrontoCola;
	KeyFrame[FrameIndex].rotBrontoPiernaDD = rotBrontoPiernaDD;
	KeyFrame[FrameIndex].rotBrontoPiernaDI = rotBrontoPiernaDI;
	KeyFrame[FrameIndex].rotBrontoPiernaTD = rotBrontoPiernaTD;
	KeyFrame[FrameIndex].rotBrontoPiernaTI = rotBrontoPiernaTI;


	FrameIndex++;
}

void definirFrames(void) {
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	if (FrameIndex == 0) {
		posX_Bronto_K = 0.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 0.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = 0.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 1) {
		posX_Bronto_K = -30.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 100.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = 20.0;
		rotBrontoPiernaDD = 20.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 20.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 2) {
		posX_Bronto_K = -60.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 200.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = -20.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 20.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 20.0;
	}
	if (FrameIndex == 3) {
		posX_Bronto_K = -90.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 300.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = 20.0;
		rotBrontoPiernaDD = 20.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 20.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 4) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = -20.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 20.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 20.0;
	}
	if (FrameIndex == 5) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = 20.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	//Empieza a bajar cuello
	if (FrameIndex == 6) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 10.0;
		rotBrontoCuello = 20.0;
		rotBrontoCola = -10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 7) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 15.0;
		rotBrontoCuello = 40.0;
		rotBrontoCola = 10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 8) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 20.0;
		rotBrontoCuello = 60.0;
		rotBrontoCola = -10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	//Su cuello se queda abajo
	if (FrameIndex == 9) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = -15.0;
		rotBrontoCuello = 80.0;
		rotBrontoCola = 10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 10) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 80.0;
		rotBrontoCola = -10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 11) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = -15;
		rotBrontoCuello = 80.0;
		rotBrontoCola = 10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	//Empieza a subir cuello
	if (FrameIndex == 12) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = -10.0;
		rotBrontoCuello = 40.0;
		rotBrontoCola = -10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 13) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = -5.0;
		rotBrontoCuello = 20.0;
		rotBrontoCola = 10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}
	if (FrameIndex == 14) {
		posX_Bronto_K = -120.0;
		posY_Bronto_K = 0.0;
		posZ_Bronto_K = 400.0;
		rotBrontoCabeza = 0.0;
		rotBrontoCuello = 0.0;
		rotBrontoCola = -10.0;
		rotBrontoPiernaDD = 0.0;
		rotBrontoPiernaDI = 0.0;
		rotBrontoPiernaTD = 0.0;
		rotBrontoPiernaTI = 0.0;
	}

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].posX_Bronto_K = posX_Bronto_K;
	KeyFrame[FrameIndex].posY_Bronto_K = posY_Bronto_K;
	KeyFrame[FrameIndex].posZ_Bronto_K = posZ_Bronto_K;
	KeyFrame[FrameIndex].rotBrontoCabeza = rotBrontoCabeza;
	KeyFrame[FrameIndex].rotBrontoCuello = rotBrontoCuello;
	KeyFrame[FrameIndex].rotBrontoCola = rotBrontoCola;
	KeyFrame[FrameIndex].rotBrontoPiernaDD = rotBrontoPiernaDD;
	KeyFrame[FrameIndex].rotBrontoPiernaDI = rotBrontoPiernaDI;
	KeyFrame[FrameIndex].rotBrontoPiernaTD = rotBrontoPiernaTD;
	KeyFrame[FrameIndex].rotBrontoPiernaTI = rotBrontoPiernaTI;

	FrameIndex++;
}
/**************************************************************************************/

void resetElements(void)
{
	/*Esto hace que, cada que se presione la tecla de reproducir (p), las posiciones tomen el valor
	cero del keyFrame*/
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	posX_Bronto_K = KeyFrame[0].posX_Bronto_K;
	posY_Bronto_K = KeyFrame[0].posY_Bronto_K;
	posZ_Bronto_K = KeyFrame[0].posZ_Bronto_K;
	rotBrontoCabeza = KeyFrame[0].rotBrontoCabeza;
	rotBrontoCuello = KeyFrame[0].rotBrontoCuello;
	rotBrontoCola = KeyFrame[0].rotBrontoCola;
	rotBrontoPiernaDD = KeyFrame[0].rotBrontoPiernaDD;
	rotBrontoPiernaDI = KeyFrame[0].rotBrontoPiernaDI;
	rotBrontoPiernaTD = KeyFrame[0].rotBrontoPiernaTD;
	rotBrontoPiernaTI = KeyFrame[0].rotBrontoPiernaTI;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	incX_Bronto_K = (KeyFrame[playIndex + 1].posX_Bronto_K - KeyFrame[playIndex].posX_Bronto_K) / i_max_steps;
	incY_Bronto_K = (KeyFrame[playIndex + 1].posY_Bronto_K - KeyFrame[playIndex].posY_Bronto_K) / i_max_steps;
	incZ_Bronto_K = (KeyFrame[playIndex + 1].posZ_Bronto_K - KeyFrame[playIndex].posZ_Bronto_K) / i_max_steps;
	incRotBrontoCabeza = (KeyFrame[playIndex + 1].rotBrontoCabeza - KeyFrame[playIndex].rotBrontoCabeza) / i_max_steps;
	incRotBrontoCuello = (KeyFrame[playIndex + 1].rotBrontoCuello - KeyFrame[playIndex].rotBrontoCuello) / i_max_steps;
	incRotBrontoCola = (KeyFrame[playIndex + 1].rotBrontoCola - KeyFrame[playIndex].rotBrontoCola) / i_max_steps;
	incRotBrontoPiernaDD = (KeyFrame[playIndex + 1].rotBrontoPiernaDD - KeyFrame[playIndex].rotBrontoPiernaDD) / i_max_steps;
	incRotBrontoPiernaDI = (KeyFrame[playIndex + 1].rotBrontoPiernaDI - KeyFrame[playIndex].rotBrontoPiernaDI) / i_max_steps;
	incRotBrontoPiernaTD = (KeyFrame[playIndex + 1].rotBrontoPiernaTD - KeyFrame[playIndex].rotBrontoPiernaTD) / i_max_steps;
	incRotBrontoPiernaTI = (KeyFrame[playIndex + 1].rotBrontoPiernaTI - KeyFrame[playIndex].rotBrontoPiernaTI) / i_max_steps;
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

			posX_Bronto_K += incX_Bronto_K;
			posY_Bronto_K += incY_Bronto_K;
			posZ_Bronto_K += incZ_Bronto_K;
			rotBrontoCabeza += incRotBrontoCabeza;
			rotBrontoCuello += incRotBrontoCuello;
			rotBrontoCola += incRotBrontoCola;
			rotBrontoPiernaDD += incRotBrontoPiernaDD;
			rotBrontoPiernaDI += incRotBrontoPiernaDI;
			rotBrontoPiernaTD += incRotBrontoPiernaTD;
			rotBrontoPiernaTI += incRotBrontoPiernaTI;


			i_curr_steps++;
		}
	}

	//xxxxxxxxxxxxxxxxxxxxxxxxx	ANIMACIONES PROYECTO	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

	if (MovRaptor) 
	{
		if (BajaCabeza)
		{
			rotaCabeza += 1.0f;
			if (rotaCabeza >= 20.0f)
			{
				SubeCabeza = true;
				BajaCabeza = false;
			}
		}
		if (SubeCabeza)
		{
			rotaCabeza -= 1.0f;
			if (rotaCabeza <= -30.0f)
			{
				SubeCabeza = false;
				BajaCabeza = true;
			}
		}
	}

	if (MovPez)
	{

		//MiVariablePez = 0.0;

		PosPez_x = 400.0f * cos(MiVariablePez);
		PosPez_z = 400.0f * sin(MiVariablePez);

		MiVariablePez += 0.02;
		OrientaCuerpoPez -= 1.15f;
		OrientaColaPez -= 1.15f;

		if (MueveColaDer)
		{
			rotaCola += 3.0;
			if (rotaCola >= 35.0f)
			{
				MueveColaDer = false;
				MueveColaIzq = true;
			}
		}
		if (MueveColaIzq)
		{
			rotaCola -= 1.0f;
			if (rotaCola <= -35.0f)
			{
				MueveColaDer = true;
				MueveColaIzq = false;
			}
		}
		
	}


	//**************************		ANIMATREN		*******************************//
	//rotaV1 += 1.0f;
	if (animaTren)
	{
		if (AvanzaZ)
		{
			if(movTren_x <= 1000.0f && movTren_z >= 520.0f) //Mientras sea menor a 1000 en X, va a moverse en Z positivo, que es en la primer vuelta
			{
				if(movTren_z <= 1619.0f)	//Mientras no llegue al límite en Z, seguirá avanzando en este eje.//antes 1615
					movTren_z += 2.0f;
			}
			if (movTren_z == 900.0f && movTren_x <=1000.0f)	//Si llega a 900 en Z, va a comenzar el avance en X y debería girar el segundo vagón
				{
				//GiraIzquierdaV2 = true;
				AvanzaX = true;
				}
			if(movTren_x >= 1600.0f && movTren_z >= 1619.0f)
			{
				AvanzaZ = false;
				AvanzaZN = true;
			}
		}

		if (AvanzaZN)
		{
			if (movTren_x >= 1650.0f && movTren_x <= 2100.0f) {	//Segunda vuelta, inicio
				if (movTren_z >= 700.0f)	//Mientras no llegue al límite en Z, se moverá en Z negativo para ir al otro kiosco
					movTren_z -= 0.8f;
				if (movTren_x >= 1400 && movTren_z >= 1200)	//Hago que se mueva más rápido
					movTren_z -= 1.6f;
			}
		}


		if (AvanzaX)
		{
			if (movTren_x <= 1200.0f)	//Mientras no llegue a este límite, va a avanzar en X, pero se debe cambiar, porque aquí va a empezar a avanzar lento en X a 1.4
			{

				if (movTren_z >= 1200 && movTren_z <= 1390)	//Si está en estos valores para Z, movemos más lento en X
				{
					movTren_x += 2.0f;
					
			
				}

				if (movTren_z >= 1450 && movTren_z <= 1620)	//Pero al pasar 1390 y al límite de movimiento en Z, X lo movemos a la velocidad normal
				{
					movTren_x += 2.0f;
				}

				/*if(movTren_x >= 600.0f && movTren_x <= 1000.0f)
					movTren_x += 2.0f;*/

			}

			if (movTren_x >= 900 && movTren_x <= 1000) 	//Movemos en el Eje X para la segunda vuelta
			{
				movTren_x += 2.0f;
			}
		}

		if (GiraIzquierdaV1)
		{
			if (movTren_z >= 1200.0f && movTren_x <= 800.0f)	//Cuando llega a 1200 en Z, empezará el giro del primer vagón
			{
					//GiraIzquierdaV1 = true;
				if (rotaV1 <= 90.0f)
				{
					rotaV1 += 0.26f;
				}
			}
			if (movTren_x >= 1650.0f && movTren_x <= 2100.0f) 
			{
				if (rotaV1 <= 180.0f)
				{
					rotaV1 += 0.3f;
					if (movTren_z <= 1350)
					{
						rotaV1 -= 0.1f;
						movTren_z -= 0.4;
					}
				}
				
			}
		}

		if (GiraIzquierdaV2)
		{
			if (movTren_z >= 1000.0f && movTren_x <= 600.0f)
			{
				if (rotaV2 <= 20.0f)
				{
					rotaV2 += 0.15f;
					//if (movTren_z >= 1580.0f)
						//rotaV2 += 0.1f;
					/*if (rotaV2 == 19.0f)
					{
						GiraIzquierdaV2 = false;
						GiraDerechaV2 = true;
					}*/
				}
				else
				{
					GiraIzquierdaV2 = false;
					GiraDerechaV2 = true;
				}
			}

			if (movTren_x >= 1500.0f && movTren_z >= 700.0f)
			{
				if (rotaV2 <= 19.0f)
				{
					rotaV2 += 0.2f;
					if (movTren_z <= 1200.0f)
						rotaV2 += 0.1f;
					/*if (rotaV2 == 19.0f)
					{
						GiraIzquierdaV2 = false;
						GiraDerechaV2 = true;
					}*/
				}
				
				if(rotaV2 == 19.0f)
				{
				//GiraIzquierdaV2 = false;
				GiraDerechaV2 = true;
					
				}
			}


		}

		if (GiraDerechaV2)
		{
			if (movTren_x >= 500 && movTren_z >= 1610 && movTren_x <= 1200)
			{
				//rotaV2 -= 0.2f;
				if (rotaV2 >= 0.0f)
				{
					rotaV2 -= 0.1f;
					if (movTren_x >= 530)
						rotaV2 -= 0.1f;
				}
				if (rotaV2 == 0){
					GiraDerechaV2 = false;
					GiraIzquierdaV2 = true;
				}
			}

			if (movTren_x >= 1500.0f && movTren_z >= 1000.0f)
				//if (movTren_x >= 530)
					
			{
				if (rotaV2 >= 0)
				{
					rotaV2 -= 0.2f;
					//GiraIzquierdaV2 = true;
					//GiraDerechaV2 = false;
				}
			}
			

		}

		

		if (GiraIzquierdaV3)
		{
			if (movTren_z >= 800.0f && movTren_x <= 1600.0f)
			{
				if (rotaV3 <= 20.0f)
				{
					rotaV3 += 0.2f;
					//if (movTren_z >= 1580.0f)
						//rotaV3 += 0.3f;
					/*if (rotaV2 == 19.0f)
					{
						GiraIzquierdaV2 = false;
						GiraDerechaV2 = true;
					}*/
				}
				else
				{
					GiraIzquierdaV3 = false;
					GiraDerechaV3 = true;
				}
			}
		}

		if (GiraDerechaV3)
		{
			if (movTren_x >= 150 && movTren_z >= 1570)
			{
				//rotaV2 -= 0.2f;
				if (rotaV3 >= 0.0f)
				{
					rotaV3 -= 0.1f;
					/*if (movTren_x >= 530)
						rotaV3 -= 0.1f;*/
				}
			}
		}
	}


	//__________________________	FIN	ANIMATREN		___________________________________//


	//**************************		ANIMACARRO		*******************************//
	if (animaCarro)
	{
		if (recorrido0_A2) {
			movAuto2_z -= 1.0f;
			orienta2 += 3.0f;
			orientaAuto2 = 90.0f;
			if (movAuto2_z <= -490.0f) {//-190
				recorrido0_A2 = false;
				recorrido0_giro_A2 = true;
			}
		}
		if (recorrido0_giro_A2) {
			movAuto2_z -= 0.6f;//2.0f    //m=-2
			movAuto2_x += 0.3f;//1.0f
			orienta2 += 3.0f;
			orientaAuto2 = 90.0f - 63.43f;
			if (movAuto2_z <= -200.0f && movAuto2_x >= 10.0f) {
				recorrido0_giro_A2 = false;
				recorrido1_A2 = true;
			}
		}
		if (recorrido1_A2) {
			movAuto2_x += 1.0f;//translacion auto completo
			orienta2 += 3.0f;//giro de llantas
			orientaAuto2 = 0.0f;
			if (movAuto2_x >= 190.0f) {
				recorrido1_A2 = false;
				recorrido1_giro_A2 = true;
			}
		}
		if (recorrido1_giro_A2) {
			movAuto2_z -= 0.6f;//2.0f    //m=-2
			movAuto2_x += 0.3f;//1.0f
			orienta2 += 3.0f;
			orientaAuto2 = 90.0f - 63.43f;
			if (movAuto2_z <= -210.0f && movAuto2_x >= 200.0f) {
				recorrido1_giro_A2 = false;
				recorrido2_A2 = true;
			}
		}
		if (recorrido2_A2) {
			movAuto2_z -= 1.0f;//translacion auto completo
			orienta2 += 3.0f;//giro de llantas
			orientaAuto2 = 90.0f;
			if (movAuto2_z <= -500.0f) {//-300
				recorrido2_A2 = false;
				recorrido3_A2 = true;
			}
		}
		if (recorrido3_A2) {
			movAuto2_z -= 1.0f;//translacion auto completo ********* 0.1f
			orienta2 += 0.5f;//giro de llantas
			orientaAuto2 = 90.0f;
			if (movAuto2_z <= -550.0f) {
				recorrido3_A2 = false;
				recorrido4_A2 = true;
			}
		}
		if (recorrido4_A2) {
			movAuto2_x += 0.5f;
			movAuto2_z = (pow(movAuto2_x - 300.0f, 2) / (45)) -770.0;//-130.0f	//-590
			orienta2 += 0.5f;//giro de llantas
			if (orientaAuto2 >= -90.0f) {
				orientaAuto2 -= 0.5f;
			}
			if (movAuto2_x >= 380.0f) {
				recorrido4_A2 = false;
			}
		}
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
		if (recorrido0_PA_JCA) {
			movPersonaAterrada_JCA_x += 0.5f;
			orientaPersonaAterrada_JCA = 90.0f;
			orientaAla1Quetzal_JCA_y -= 0.2f;
			orientaAla2Quetzal_JCA_y += 0.2f;
			orientaCentroQuetzal_JCA_x -= 0.1f;
			if (movPersonaAterrada_JCA_x >= 50.0f) {
				recorrido0_PA_JCA = false;
				recorrido1_PA_JCA = true;
				miVariable1_JCA = 0.0f;
				orientaAla1Quetzal_JCA_y = 0.0f;
				orientaAla2Quetzal_JCA_y = 0.0f;
				orientaCentroQuetzal_JCA_x = 0.0f;
				//movPersonaAterrada_JCA_x = 0.0f;
			}
		}
		if (recorrido1_PA_JCA) {

			movPersonaAterrada_JCA_x = 50.0f * cos(miVariable1_JCA);//sqrt( 1 - pow(movPersonaAterrada_JCA_z,2) );
			movPersonaAterrada_JCA_z = 50.0f * sin(miVariable1_JCA);
			miVariable1_JCA += 0.02f;
			//orientaPersonaAterrada_JCA = 0.0f - (miVariable1 * 30.0);

			if (miVariable1_JCA <= 0.5f) {
				orientaPersonaAterrada_JCA = 0.0f;
				orientaAla1Quetzal_JCA_y += 0.2f;
				orientaAla2Quetzal_JCA_y -= 0.2f;
				orientaCentroQuetzal_JCA_x += 0.1f;
			}
			if (miVariable1_JCA > 0.5f && miVariable1_JCA <= 2.0f) {
				orientaPersonaAterrada_JCA = -90.0f;
				orientaAla1Quetzal_JCA_y -= 0.2f;
				orientaAla2Quetzal_JCA_y += 0.2f;
				orientaCentroQuetzal_JCA_x -= 0.1f;
			}
			if (miVariable1_JCA > 2.0f && miVariable1_JCA <= 4.0f) {
				orientaPersonaAterrada_JCA = -180.0f;
				orientaAla1Quetzal_JCA_y += 0.2f;
				orientaAla2Quetzal_JCA_y -= 0.2f;
				orientaCentroQuetzal_JCA_x += 0.1f;
			}
			if (miVariable1_JCA > 4.0f && miVariable1_JCA <= 5.2f) {
				orientaPersonaAterrada_JCA = -270.0f;
				orientaAla1Quetzal_JCA_y -= 0.2f;
				orientaAla2Quetzal_JCA_y += 0.2f;
				orientaCentroQuetzal_JCA_x -= 0.1f;
			}
			if (miVariable1_JCA > 5.2f && miVariable1_JCA <= 7.0f) {
				orientaPersonaAterrada_JCA = 0.0f;
				orientaAla1Quetzal_JCA_y += 0.2f;
				orientaAla2Quetzal_JCA_y -= 0.2f;
				orientaCentroQuetzal_JCA_x += 0.1f;
			}

			if (miVariable1_JCA > 7.0f) {
				recorrido0_PA_JCA = true;
				recorrido1_PA_JCA = false;
				movPersonaAterrada_JCA_x = 0.0f;
				movPersonaAterrada_JCA_z = 0.0f;
				orientaAla1Quetzal_JCA_y = 0.0f;
				orientaAla2Quetzal_JCA_y = 0.0f;
				orientaCentroQuetzal_JCA_x = 0.0f;
			}
		}
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
	//Model Auto2("resources/objects/Auto2JCA/Auto2_JCA.obj");
	Model Auto2JCA("resources/objects/Auto2JCA/Auto2_JCA.obj");
	Model LlantaAuto2JCA("resources/objects/Auto2JCA/LlantaAuto2_JCA_r.obj");

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
	Model Raptor("resources/objects/Dinos/Raptor/Raptor.obj");
	Model CabezaRap("resources/objects/Dinos/Raptor/cabeza.obj");
	Model CuerpoRap("resources/objects/Dinos/Raptor/cuerpo.obj");
	Model CuerpoPez("resources/objects/Dinos/Pez/Cuerpo.obj");
	Model ColaPez("resources/objects/Dinos/Pez/Cola.obj");

//	Model Quetzal("resources/objects/Dinos/Quetzal/13623_Quetzalcoatlus_v1_L2.obj");

	//Helicoptero
	Model Helicoptero("resources/objects/Helicoptero/Helicoptero.obj");

	Model VelociBone("resources/objects/VelociraptorBone/VelociBone.obj");
	//BRONTOSAURIO
	Model TorsoBronto("resources/objects/Brontosaurio/Torso_JCA.obj");
	Model CabezaBronto("resources/objects/Brontosaurio/Cabeza_JCA.obj");
	Model CuelloBronto("resources/objects/Brontosaurio/Cuello_JCA.obj");
	Model ColaBronto("resources/objects/Brontosaurio/Cola_JCA.obj");
	Model PiernaDD_Bronto("resources/objects/Brontosaurio/PiernaDD_JCA.obj");
	Model PiernaDI_Bronto("resources/objects/Brontosaurio/PiernaDI_JCA.obj");
	Model PiernaTD_Bronto("resources/objects/Brontosaurio/PiernaTD_JCA.obj");
	Model PiernaTI_Bronto("resources/objects/Brontosaurio/PiernaTI_JCA.obj");


	// Animación ataque
	Model QuetzalCentroJCA("resources/objects/Quetzal/QuetzalCentro.obj");
	Model QuetzalAla1JCA("resources/objects/Quetzal/QuetzalAla1.obj");
	Model QuetzalAla2JCA("resources/objects/Quetzal/QuetzalAla2.obj");

	ModelAnim PersonaAterradaJCA("resources/objects/PersonaAterrada/Terrified.dae");
	PersonaAterradaJCA.initShaders(animShader.ID);

	ModelAnim PersonaCorriendoJCA("resources/objects/PersonaCorriendo/GoofyRunning.dae");
	PersonaCorriendoJCA.initShaders(animShader.ID);


	//TREX
	Model Trex("resources/objects/Trex/Trex2.obj");


	//TREN
	Model Riel("resources/objects/Vias/riel1.obj");
	Model Tren("resources/objects/Tren/Tren.obj");
	Model Vagon1("resources/objects/Tren/Vag1.obj");
	Model Vagon2("resources/objects/Tren/Vag2.obj");
	Model Vagon3("resources/objects/Tren/Vag3.obj");

	//METEORITO
	Model Meteor("resources/objects/Meteorito/Meteorito.obj");

	//ModelAnim Meteorito("resources/objects/Meteorito/Asteroide.dae");	//agrego al meteorito
	//Meteorito.initShaders(animShader.ID);

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

		KeyFrame[i].posX_Bronto_K = 0;
		KeyFrame[i].posY_Bronto_K = 0;
		KeyFrame[i].posZ_Bronto_K = 0;
		KeyFrame[i].rotBrontoCabeza = 0;
		KeyFrame[i].rotBrontoCuello = 0;
		KeyFrame[i].rotBrontoCola = 0;
		KeyFrame[i].rotBrontoPiernaDD = 0;
		KeyFrame[i].rotBrontoPiernaDI = 0;
		KeyFrame[i].rotBrontoPiernaTD = 0;
		KeyFrame[i].rotBrontoPiernaTI = 0;

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
		glm::mat4 tempV1 = glm::mat4(1.0f);
		glm::mat4 tempV2 = glm::mat4(1.0f);
		glm::mat4 tmpCA1 = glm::mat4(1.0f);//Temporal para cubo auto 1
		glm::mat4 tmpCA2 = glm::mat4(1.0f);//Temporal para quetzal
		
		/*****************MODIFICACIONES DE JESS*******************************************************/
		//glm::mat4 tmpCA2 = glm::mat4(1.0f);//Temporal para quetzal
		glm::mat4 tmpCA3 = glm::mat4(1.0f);//Temporal para brontosaurio
		glm::mat4 tmpCA4 = glm::mat4(1.0f);//Temporal para cuello brontosaurio
		/**********************************************************************************************/


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


		// Ataque

		//Animación para persona que se asusta y corre
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posPersonaAterrada_JCA_x + movPersonaAterrada_JCA_x, posPersonaAterrada_JCA_y + movPersonaAterrada_JCA_y, posPersonaAterrada_JCA_z + movPersonaAterrada_JCA_z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(escalaPersonaAterrada_JCA));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(orientaInicialPersonaAterrada_JCA + orientaPersonaAterrada_JCA), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		if (animaAtaque) {
			//Persona corriendo
			PersonaCorriendoJCA.Draw(animShader);
		}
		else {
			//Persona aterrada
			PersonaAterradaJCA.Draw(animShader);
		}





		// -------------------------------------------------------------------------------------------------------------------------
		// Fin Animaciones Proyecto
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//---------------------------------------------VELOCIBONE---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-850.0f, 0.0f, 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		VelociBone.Draw(staticShader);

		//---------------------------------------------FIN VELOCIBONE---------------------------------------------------//
		// -------------------------------------------------------------------------------------------------------------------------



		// Modelos Proyecto
		// -------------------------------------------------------------------------------------------------------------------------
	



		//--------------------------------------------- TREN---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 45.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Riel.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 350.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		//Tren.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x, 11.0f, movTren_z));//150,11,520
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x, 11.0f, movTren_z));//150,11,520
		tempV1 = model = glm::rotate(model, glm::radians(rotaV1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Vagon1.Draw(staticShader);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x, 11.0f, movTren_z));//520 la chidota
		model = glm::translate(tempV1, glm::vec3(0.0f,0.0f,0.0f));//prueba
		tempV2 = model = glm::rotate(model, glm::radians(rotaV2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Vagon2.Draw(staticShader);

		//temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x, 11.0f, 621.0f));//520+101
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x, 11.0f, 621.0f));//520+101
		model = glm::translate(tempV2, glm::vec3(0.0f, 0.0f, 101.0f));//621.0f
		model = glm::rotate(model, glm::radians(rotaV3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Vagon3.Draw(staticShader);
		//--------------------------------------------- FIN DE TREN---------------------------------------------------//



		//---------------------------------------------AUTO_2---------------------------------------------------//

				//Auto 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posAuto2_x - movAuto2_x, posAuto2_y + movAuto2_y, posAuto2_z - movAuto2_z));
		tmpCA1 = model = glm::rotate(model, glm::radians(orientaAuto2 + orientaInicial2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaAuto2_JCA));
		staticShader.setMat4("model", model);
		Auto2JCA.Draw(staticShader);

		//Llanta 1 para Auto 2 (delantera izquierda)
		model = glm::translate(tmpCA1, glm::vec3(19.0f, 3.0f, 41.5f));
		model = glm::rotate(model, glm::radians(orienta2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaLlantaAuto2_JCA));
		staticShader.setMat4("model", model);
		LlantaAuto2JCA.Draw(staticShader);

		//Llanta 2 para Auto 2 (delantera derecha)
		model = glm::translate(tmpCA1, glm::vec3(-19.0f, 3.0f, 41.0f));
		model = glm::rotate(model, glm::radians(orienta2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaLlantaAuto2_JCA));
		staticShader.setMat4("model", model);
		LlantaAuto2JCA.Draw(staticShader);

		//Llanta 3 para Auto 2 (trasera izquierda)
		model = glm::translate(tmpCA1, glm::vec3(19.0f, 3.0f, -30.0f));
		model = glm::rotate(model, glm::radians(orienta2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaLlantaAuto2_JCA));
		staticShader.setMat4("model", model);
		LlantaAuto2JCA.Draw(staticShader);

		//Llanta 4 para Auto 2 (trasera derecha)
		model = glm::translate(tmpCA1, glm::vec3(-19.5f, 3.0f, -30.0f));
		model = glm::rotate(model, glm::radians(orienta2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaLlantaAuto2_JCA));
		staticShader.setMat4("model", model);
		LlantaAuto2JCA.Draw(staticShader);


		//--------------------------------------------- FIN DE AUTO_2---------------------------------------------------//

		//--------------------------------------------- BRONTOSAURIO---------------------------------------------------//

		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posX_Bronto + posX_Bronto_K, posY_Bronto + posY_Bronto_K, posZ_Bronto + posZ_Bronto_K));
		tmpCA3 = model = glm::rotate(model, glm::radians(orientaInicialBrontosaurio_JCA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		TorsoBronto.Draw(staticShader);

		//Cuello
		model = glm::translate(tmpCA3, glm::vec3(0.0f, 30.0f, 83.0f));//0.0,30.0,77.0
		tmpCA4 = model = glm::rotate(model, glm::radians(rotBrontoCuello), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		CuelloBronto.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmpCA4, glm::vec3(0.0f, 235.0f, 195.0f));//270
		model = glm::rotate(model, glm::radians(rotBrontoCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		CabezaBronto.Draw(staticShader);

		//Cola
		model = glm::translate(tmpCA3, glm::vec3(0.0f, 0.0f, -78.0f));
		model = glm::rotate(model, glm::radians(rotBrontoCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		ColaBronto.Draw(staticShader);

		//Pierna DD
		model = glm::translate(tmpCA3, glm::vec3(-35.0f, -18.0f, 60.0f));
		model = glm::rotate(model, glm::radians(rotBrontoPiernaDD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		PiernaDD_Bronto.Draw(staticShader);

		//Pierna DI
		model = glm::translate(tmpCA3, glm::vec3(35.0f, -18.0f, 60.0f));
		model = glm::rotate(model, glm::radians(rotBrontoPiernaDI), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		PiernaDI_Bronto.Draw(staticShader);

		//Pierna TD
		model = glm::translate(tmpCA3, glm::vec3(-35.0f, 2.0f, -60.0f));
		model = glm::rotate(model, glm::radians(rotBrontoPiernaTD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		PiernaTD_Bronto.Draw(staticShader);

		//Pierna TI
		model = glm::translate(tmpCA3, glm::vec3(35.0f, 2.0f, -60.0f));
		model = glm::rotate(model, glm::radians(rotBrontoPiernaTI), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBrontosaurio_JCA));
		staticShader.setMat4("model", model);
		PiernaTI_Bronto.Draw(staticShader);

		//--------------------------------------------- FIN	BRONTOSAURIO---------------------------------------------------//


		//--------------------------------------------- ATAQUE Quetzal---------------------------------------------------//

		//Quetzal
		//Centro
		tmpCA2 = model = glm::translate(glm::mat4(1.0f), glm::vec3(posQuetzal_JCA_x, posQuetzal_JCA_y, posQuetzal_JCA_z));
		model = glm::rotate(tmpCA2, glm::radians(orientaCentroQuetzal_JCA_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaCentroQuetzal_JCA_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaQuetzal_JCA));
		staticShader.setMat4("model", model);
		QuetzalCentroJCA.Draw(staticShader);

		//Ala 1
		model = glm::translate(tmpCA2, glm::vec3(-23.0f, 0.0f, -6.0f));//-11 0 -3
		model = glm::rotate(model, glm::radians(orientaCentroQuetzal_JCA_y + orientaAla1Quetzal_JCA_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaCentroQuetzal_JCA_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaQuetzal_JCA));
		staticShader.setMat4("model", model);
		QuetzalAla1JCA.Draw(staticShader);

		//Ala 2
		model = glm::translate(tmpCA2, glm::vec3(-23.0f, 0.0f, 17.0f));//-11 0 9
		model = glm::rotate(model, glm::radians(orientaCentroQuetzal_JCA_y + orientaAla2Quetzal_JCA_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaCentroQuetzal_JCA_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaQuetzal_JCA));
		staticShader.setMat4("model", model);
		QuetzalAla2JCA.Draw(staticShader);



		//--------------------------------------------- FIN	ATAQUE---------------------------------------------------//

		//--------------------------------------------- TREX---------------------------------------------------//

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, 2700.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		Trex.Draw(staticShader);

		//--------------------------------------------- FIN	TREX---------------------------------------------------//


		
		//--------------------------------------------- EDIFICIOS---------------------------------------------------//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2150.0f, 0.0f,2150.0f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0));
		staticShader.setMat4("model", model);
		Hotel.Draw(staticShader);	//Edificio Hotel izquierdo

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1450.0f, 0.0f, 900.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1050.0f, 0.0f, -615.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaEntrada_JCA));
		staticShader.setMat4("model", model);
		PuertaDer.Draw(staticShader);	//Puerta Derecha

		model = glm::translate(glm::mat4(1.0f), glm::vec3(820.0f, 0.0f, -615.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(630.0f, 0.0f, 2050.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		ArbolesLinea.Draw(staticShader);	//


		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.0f, 500.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Raptor.Draw(staticShader);	//Raptor dino extra

		model = glm::translate(glm::mat4(1.0f), glm::vec3(PosIniPez_x + PosPez_x, 20.0f, PosIniPez_z + PosPez_z));
		model = glm::rotate(model, glm::radians(rotaCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(OrientaColaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		//ColaPez.Draw(staticShader);	//Pez dino extra

		model = glm::translate(glm::mat4(1.0f), glm::vec3(PosIniPez_x + PosPez_x, 20.0f, PosIniPez_z + PosPez_z));	//600	20	600 PosIniPez_z +
		model = glm::rotate(model, glm::radians(OrientaCuerpoPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		//CuerpoPez.Draw(staticShader);	//Pez dino extra


		//--------------------------------------------- FIN DE NATURALEZA Y DINOSAURIOS---------------------------------------------------//


		//--------------------------------------------- HELICOPTERO---------------------------------------------------//



		model = glm::translate(glm::mat4(1.0f), glm::vec3(930.0f, 0.0f, -1800.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		Helicoptero.Draw(staticShader);	
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
		tmpCA1 = model = glm::translate(glm::mat4(1.0f), glm::vec3(1800.0f, -1.0f, -1600.0f));
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 800.0f, 1000.0f));
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
	//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		//lightPosition.y--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion ^= true;
	}

	//Teclas animaciones Proyecto
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animaTren ^= true;
		AvanzaZ = true;
		AvanzaZN = false;
		AvanzaX = false;
		GiraIzquierdaV1 = true;
		GiraDerechaV1 = false;
		GiraIzquierdaV2 = true;
		GiraDerechaV2 = false;
		GiraIzquierdaV3 = true;
		GiraDerechaV3 = false;

		movTren_x = 150.0f;
		movTren_z = 520.0f;
		movTren_xV3 = 150.0f;//+101
		movTren_zV3 = 621.0f;
		
		rotaV1 = 0.0f;
		rotaV2 = 0.0f;
		rotaV3 = 0.0f;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		animaCarro^= true;
		recorrido0_A2 = true,
		recorrido0_giro_A2 = false,
		recorrido1_A2 = false,
		recorrido1_giro_A2 = false,
		recorrido2_A2 = false,
		recorrido3_A2 = false,
		recorrido4_A2 = false;
		movAuto2_z = 0.0f;
		movAuto2_x = 0.0f;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		//animaBronto ^= true;
		definirFrames();
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		animaAtaque ^= true;
		recorrido0_PA_JCA = true;
		recorrido1_PA_JCA = false;
		movPersonaAterrada_JCA_x = 0.0f;
		movPersonaAterrada_JCA_z = 0.0f;
		miVariable1_JCA = 0.0f;
		orientaAla1Quetzal_JCA_y = 0.0f;
		orientaAla2Quetzal_JCA_y = 0.0f;
		orientaCentroQuetzal_JCA_x = 0.0f;
		orientaPersonaAterrada_JCA = 0.0f;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		animaMeteor ^= true;
	}

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		MovPez ^= true;
		PosPez_x = 0;
		posAuto2_z = 0;
		PosIniPez_x = 600.0f;
		PosIniPez_z = 600.0f;
		rotaCola = 0.0f;
		rotaCabeza = 0.0f;
		OrientaCuerpoPez = 0.0f;
		OrientaColaPez = 0.0f;
		MiVariablePez = 0.0f;
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
