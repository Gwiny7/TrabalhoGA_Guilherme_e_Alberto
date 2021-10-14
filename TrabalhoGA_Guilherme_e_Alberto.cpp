#include "test.h"
#include <cmath>
#include <algorithm>
#include <ctime>
#include "imgui/imgui.h"



struct Corrente {
	b2Body* Contas[11];
	b2RevoluteJoint* Juntas[11];
	b2Body* Base;
	b2RevoluteJoint* LigaB;
	b2RevoluteJoint* Liga;
	float X;
	float Y;
};

struct Rolante {
	b2Body* Centro;
	b2Body* Topo;
	b2Body* Chao;
	b2WheelJoint* Rolo;
	b2RevoluteJoint* Liga;
	float X;
	float Y;
};

struct Disputa {
	b2Body* Peso1;
	b2Body* Peso2;
	b2PulleyJoint* Roldana;
	float X;
	float Y;
};

struct Vitoria {
	b2Body* Vit;
	b2Body* Mastro;
	b2Body* Band;
	b2Body* Ponta;
};

class GA_teste : public Test //voc? cria a sua classe derivada da classe base Test
{
	
public:
	

	float inicialX = -293.0f;
	float inicialY = 67.0f;
	b2Body* player = criarCaixa(inicialX, inicialY, 2.0f, 2.0f, 10.f, 0.3f, 0.3f,false,true,2);
	b2Body* vetor = nullptr;
	Camera* box2dCamera;
	float angulo = 45.0f;
	float intensidade = 0.0f;
	int pulos = 0;
	float maxPulos = 5;
	b2Vec2 gravity;
	float gravityValue = -40.0f;
	bool cameraFollowCheck = true;
	

	b2Vec2 mouseWorld;
	b2MouseJoint* mouseJoint;

	GA_teste() {
		// Aqui no construtor voc? inicializa a cena
		//box2d box2d = new PBox2D(this);


		srand(time(0));
		bool spawn = true;
		float spX, spY;
		float dSpX[7], dSpY[7], rSpX[5], rSpY[5], cSpX[10], cSpY[10];
		dSpX[0] = 0;
		dSpY[0] = 0;

		rSpX[0] = 0;
		rSpY[0] = 0;

		cSpX[0] = 0;
		cSpY[0] = 0;
		gravity = b2Vec2(0.0f, gravityValue);
		m_world->SetGravity(gravity);

		b2Body* chao = criarCaixa(0.0f, -49.0f, 600.0f, 2.0f,10.0f,0.5f,0.0f,false,false,3);
		b2Body* platInicial = criarCaixa(-293.0f, 65.0f, 14.0f, 2.0f, 10.0f, 0.9f);

		chao->SetType(b2_staticBody);
		platInicial->SetType(b2_staticBody);
		criarLimites(100.0f, -50.0f, 300.0f, -300.0f);
		vetor = criarVetor();		

		Disputa* dis[7];
		Rolante* rol[5];
		Corrente* cor[10];
		for (int i = 0; i < 7; i++) {
			do {
				spawn = true;
				spX = rand() % 530 - 262.5f;
				spY = rand() % 114 - 39.0f;
				
					for (int o = 0; o < i; o++) {
						if (spX <= dSpX[o] + 50.0f && spX >= dSpX[o] - 50.0f) {
							if (spY <= dSpY[o] + 50.0f && spY >= dSpY[o] - 50.0f) {
								spawn = false;
							}
						}
						if (spawn == false)
							break;
					}
				
			} while (spawn == false);
			dSpX[i] = spX;
			dSpY[i] = spY;
			dis[i] = criarDisputa(spX, spY);
		}

		for (int i = 0; i < 5; i++) {
			do {
				spawn = true;
				spX = rand() % 494 - 240.0f;
				spY = rand() % 114 - 31.0f;
				for (int o = 0; o < 10; o++) {
					if (spX <= dSpX[o] + 30.0f && spX >= dSpX[o] - 30.0f) {
						if (spY <= dSpY[o] + 30.0f && spY >= dSpY[o] - 30.0f) {
							spawn = false;
						}
					}					
					if (spawn == false)
						break;
				}
				for (int o = 0; o < i; o++) {
					if (spX <= rSpX[o] + 75.0f && spX >= rSpX[o] - 75.0f) {
						if (spY <= rSpY[o] + 75.0f && spY >= rSpY[o] - 75.0f) {
							spawn = false;
						}
					}
					if (spawn == false)
						break;
				}

			} while (spawn == false);
			rSpX[i] = spX;
			rSpY[i] = spY;
			rol[i] = criarRolante(spX, spY);
		}

		for (int i = 0; i < 10; i++) {
			do {
				spawn = true;
				spX = rand() % 540 - 270.0f;
				spY = rand() % 109 - 19.75f;
				for (int o = 0; o < 7; o++) {
					if (spX <= dSpX[o] + 25.0f && spX >= dSpX[o] - 25.0f) {
						if (spY <= dSpY[o] + 25.0f && spY >= dSpY[o] - 25.0f) {
							spawn = false;
						}
					}
					if (spawn == false)
						break;
				}
				for (int o = 0; o < 5; o++) {
					if (spX <= rSpX[o] + 40.0f && spX >= rSpX[o] - 40.0f) {
						if (spY <= rSpY[o] + 75.0f && spY >= rSpY[o] - 75.0f) {
							spawn = false;
						}
					}
					if (spawn == false)
						break;
				}

				for (int o = 0; o < 5; o++) {
					if (spX <= cSpX[o] + 20.0f && spX >= cSpX[o] - 20.0f) {
						if (spY <= cSpY[o] + 30.0f && spY >= cSpY[o] - 30.0f) {
							spawn = false;
						}
					}
					if (spawn == false)
						break;
				}
			} while (spawn == false);
			cSpX[i] = spX;
			cSpY[i] = spY;
			cor[i] = criarCorrente(spX, spY);
		}

		float posV = rand() % 110 - 30;

		Vitoria* vit = criarVitoria(290, posV);		
	}

	Vitoria* criarVitoria(float posX, float posY) {
		Vitoria* plat = new Vitoria;
		
		b2Body* vit = criarCaixa(posX, posY, 20, 2, 10, 0.9, 0.0, false, false, 4);
		vit->SetType(b2_staticBody);
		b2Body* mastro = criarCaixa(posX + 7, posY + 8.5f, 2, 15, 10, 0.9, 0.0, true, false);
		mastro->SetType(b2_staticBody);
		b2Body* band = criarCaixa(posX + 3, posY + 13.5, 8, 4, 10, 0.9, 0.0, true, false);
		band->SetType(b2_staticBody);
		b2Body* ponta = criarCirculo(posX + 7, posY + 17, 1.5f, 10, 0.3, 0, true);
		ponta->SetType(b2_staticBody);

		plat->Vit = vit;
		plat->Mastro = mastro;
		plat->Band = band;
		plat->Ponta = ponta;

		return plat;
	}

	void UpdateUI() override
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 100.0f));
		ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
		ImGui::Begin("Joint Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		if (ImGui::SliderFloat("Gravidade", &gravityValue, -100.0f, 100.0f, "%.0f"))
		{
			gravity = b2Vec2(0.0f, gravityValue);

			m_world->SetGravity(gravity);
		}

		if (ImGui::SliderFloat("Saltos", &maxPulos, 1.0f, 10.0f, "%.0f"))
		{
			
		}

		if (ImGui::Checkbox("CameraFollow", &cameraFollowCheck))
		{
		}

		ImGui::End();
	}

	b2Body* criarCaixa(float posX, float posY, float tamanhoX, float tamanhoY, float densidade = 10.f, float atrito = 0.5f, float restituicao = 0.f, bool sensor = false, bool trava = false, int tag = 1)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(posX, posY);

		bodyDef.fixedRotation = trava;

		b2Body* body = m_world->CreateBody(&bodyDef);
		

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(tamanhoX / 2, tamanhoY / 2);

		b2FixtureDef fixtureDef;

		fixtureDef.isSensor = sensor;

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = densidade;

		fixtureDef.friction = atrito;

		fixtureDef.restitution = restituicao;

		//Adicionei +1 parametro no final, um int tag que vai receber o indice do tipo do objeto, se ele n for especificado ja começa como uma plataforma.
		//Indices: 1: Plataforma
		//         2: Player
		//         3: Espinhos
		//         4: Bandeira(Vitória)

		//As interações vão estar dentro do collisionManager().

		fixtureDef.userData.pointer = tag;

		body->CreateFixture(&fixtureDef);

		return body;
	}

	b2Body* criarCirculo(float posX, float posY, float radius, float den = 10.f, float fric = 0.3f, float rest = 0.0f, bool sensor = false) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(posX, posY);
		b2Body* body = m_world->CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2CircleShape dynamicBox;
		dynamicBox.m_radius = radius;

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;

		fixtureDef.isSensor = sensor;

		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = den;

		// Override the default friction.
		fixtureDef.friction = fric;

		fixtureDef.restitution = rest;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
		return body;
	}

	void criarLinha(float posX, float posY, float posX2, float posY2)
	{
		b2BodyDef bodyDef;
		b2Body* body = m_world->CreateBody(&bodyDef);

		b2EdgeShape dynamicBox;
		dynamicBox.m_vertex1 = b2Vec2(posX, posY);
		dynamicBox.m_vertex2 = b2Vec2(posX2, posY2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		body->CreateFixture(&fixtureDef);
	}

	b2Body* criarVetor()
	{

		b2BodyDef bodyDef;
		b2Body* body = m_world->CreateBody(&bodyDef);
		body->SetEnabled(false);

		return body;
	}

	void criarLimites(float top, float bottom, float maxright, float maxleft)
	{
		criarLinha(maxleft, top, maxright, top);
		criarLinha(maxright, top, maxright, bottom);
		criarLinha(maxright, bottom, maxleft, bottom);
		criarLinha(maxleft, bottom, maxleft, top);

	}
	//Função para criação de juntas de distância
	b2DistanceJoint* criarDistance(b2Body* b1, b2Body* b2, b2Vec2 an, b2Vec2 an2) {
		b2DistanceJointDef dJointDef;
		dJointDef.Initialize(b1, b2, an, an2);

		b2DistanceJoint* dJoint = (b2DistanceJoint*)m_world->CreateJoint(&dJointDef);
		return dJoint;
	}

	//Função para criação de rodas
	b2WheelJoint* criarWheel(b2Body* b1, b2Body* b2, b2Vec2 an, b2Vec2 ax = b2Vec2(0, 1)) {
		b2WheelJointDef wJointDef;
		wJointDef.Initialize(b1, b2, an, ax);
		wJointDef.lowerTranslation = 0.0;
		wJointDef.upperTranslation = 0.0;
		wJointDef.enableLimit = true;

		b2WheelJoint* wJoint = (b2WheelJoint*)m_world->CreateJoint(&wJointDef);
		return wJoint;
	}

	//Função para criação de juntas revolutas
	b2RevoluteJoint* criarRevolute(b2Body* b1, b2Body* b2, b2Vec2 an, float lAngle = 0.0f, float uAngle = 0.0f) {
		b2RevoluteJointDef rJointDef;
		rJointDef.Initialize(b1, b2, an);
		rJointDef.lowerAngle = degreesToRadians(lAngle);
		rJointDef.upperAngle = degreesToRadians(uAngle);
		rJointDef.enableLimit = true;

		b2RevoluteJoint* rJoint = (b2RevoluteJoint*)m_world->CreateJoint(&rJointDef);
		return rJoint;
	}

	b2PrismaticJoint* criarPrismatic(b2Body* b1, b2Body* b2, b2Vec2 an, b2Vec2 ax, bool limit = false, float lTrans = 0.0f, float uTrans = 0.0f) {

		b2PrismaticJointDef pJointDef;
		pJointDef.Initialize(b1, b2, an, ax);
		pJointDef.collideConnected = false;

		//Para limitar a transla��o
		if (limit) {
			pJointDef.lowerTranslation = uTrans;
			pJointDef.upperTranslation = lTrans;
			pJointDef.enableLimit = true;
		}
		//Para criar um motor
		//pJointDef.maxMotorForce = 50.0f; //N
		//pJointDef.motorSpeed = -10.0f; //metros por segundo 
		//pJointDef.enableMotor = true;

		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)m_world->CreateJoint(&pJointDef);
		
		return pJoint;
	}

	b2PulleyJoint* criarPulley(b2Body* b1, b2Body* b2, b2Vec2 an1, b2Vec2 an2, b2Vec2 lAn1, b2Vec2 lAn2, float comp) {
		b2PulleyJointDef joint_def;
		joint_def.Initialize(b1, b2, an1,
			an2, lAn1, lAn2, comp);
		b2PulleyJoint* joint = (b2PulleyJoint*)m_world->CreateJoint(&joint_def);
		return joint;
	}

	Corrente* criarCorrente(float posx, float posy ) {
		Corrente* plat = new Corrente;
		
		b2Body* contas[11];
		b2RevoluteJoint* juntas[11];
		b2Body* base;

		contas[0] = criarCaixa(posx, posy, 5.0f, 2.0f, 10, 0.5, 0.0, true);
		contas[0]->SetType(b2_staticBody);
		
		plat->Contas[0] = contas[0];
		
		for (int i = 1; i < 11; i++)
		{
			contas[i] = criarCaixa(posx, posy - i * 1.75f, 2.0f, 2.0f, 10.0f, 0.5f, 0.0f, true);
			//contas[i]->SetType(b2_staticBody);
			plat->Contas[i] = contas[i];
		}

		base = criarCaixa(posx, contas[10]->GetWorldCenter().y - 1.75, 12.0f, 2.0f, 10, 1.0f, 0.0f);
		plat->Base = base;
		
		b2RevoluteJoint* ligaB = criarRevolute(contas[10], base, contas[10]->GetWorldPoint(b2Vec2(0.0f, -0.85f)));
		plat->LigaB = ligaB;

		for (int i = 0; i < 9; i++)
		{
			juntas[i] = criarRevolute(contas[i+1], contas[i + 2], contas[i+1]->GetWorldPoint(b2Vec2(0.0f, -0.85f)), -10.f, 10.f);
			plat->Juntas[i] = juntas[i];
		}

		b2RevoluteJoint* liga = criarRevolute(contas[0], contas[1], contas[0]->GetWorldPoint(b2Vec2(0.0f, -0.85f)));
		plat->Liga = liga;

		plat->X = posx;
		plat->Y = posy;
		
		return plat;
	}

	Rolante* criarRolante(float posx, float posy) {
		Rolante* plat = new Rolante;

		b2Body* centro = criarCirculo(posx, posy, 2.0f);
		b2Body* topo = criarCaixa(posx, posy + 14.0, 12.0f, 2.0f, 10, 1.0f, 0.0f);
		b2Body* chao = criarCaixa(posx, posy - 14.0, 12.0f, 2.0f, 10, 1.0f, 0.0f);

		plat->Centro = centro;
		plat->Topo = topo;
		plat->Chao = chao;

		centro->SetType(b2_staticBody);
		//topo->SetType(b2_staticBody);
		//chao->SetType(b2_staticBody);
		b2WheelJoint* rolo = criarWheel(centro, topo, centro->GetWorldCenter());
		b2RevoluteJoint* liga = criarRevolute(topo, chao, chao->GetWorldCenter());

		plat->Rolo = rolo;
		plat->Liga = liga;

		plat->X = posx;
		plat->Y = posy;

		//liga->EnableLimit(false);

		rolo->EnableMotor(true);
		rolo->SetMaxMotorTorque(3000.0f);
		rolo->SetMotorSpeed(100.0f);
	
		return plat;
	}

	Disputa* criarDisputa(float posx, float posy) {
		Disputa* plat = new Disputa;
		
		b2Body* peso1 = criarCaixa(posx - 9.0f, posy, 9.0f, 2.0f, 10, 1.0f, 0.0f, false, true);
		b2Body* peso2 = criarCaixa(posx + 9.0f, posy, 9.0f, 2.0f, 10, 1.0f, 0.0f, false, true);

		plat->Peso1 = peso1;
		plat->Peso2 = peso2;

		plat->X = posx;
		plat->Y = posy;

		b2PulleyJoint* roldana = criarPulley(peso1, peso2, peso1->GetWorldPoint(b2Vec2(0.0f, 15.0f)), peso2->GetWorldPoint(b2Vec2(0.0f, 15.0f)), peso1->GetWorldPoint(b2Vec2(0.0f, 1.9f)), peso2->GetWorldPoint(b2Vec2(0.0f, 1.9f)), 1.0f);
		plat->Roldana = roldana;

		return plat;
	}

	void Step(Settings& settings) override
	{
		
		Inputs();
		if (player)
			refreshVetor();

		if(cameraFollowCheck)
			cameraFollow();

		collisionManager();
		//Chama o passo da simula??o e o algoritmo de rendering
		Test::Step(settings);



		//// We are going to destroy some bodies according to contact
		//// points. We must buffer the bodies that should be destroyed
		//// because they may belong to multiple contact points.
		

		//show some text in the main screen
		g_debugDraw.DrawString(5, m_textLine, "Controles: \nUse o mouse para definir a direcao de pulo;\nPressione ESPACO para definir a forca de salto;\nSolte ESPACO para pular.");
		m_textLine += 15;
	}

	static Test* Create()  //a classe Test que instancia um objeto da sua nova classe
						   //o autor da Box2D usa um padr?o de projeto chamado Factory
						   //para sua arquitetura de classes
	{
		return new GA_teste;
	}

	void Inputs()
	{
		Charge(maxPulos);	
		
	}

	void Charge(int pulosMax)
	{
		glfwSetInputMode(g_mainWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		if (glfwGetKey(g_mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (pulos < pulosMax) {
				intensidade = std::min(intensidade + 50.0f, 2000.0f);
				if (glfwGetKey(g_mainWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
				{
					if (player)
					{
						player->ApplyLinearImpulseToCenter(getVetor(), true);
						intensidade = 0.0f;
						pulos++;
					}
				}
			}
			
		}
	}

	void collisionManager()
	{
	
		for (int32 i = 0; i < m_pointCount; ++i)  //Percorre todos os pontos
		{
			ContactPoint* point = m_points + i;  //Detecta que houve colisão

			b2Fixture* body1 = point->fixtureA;   //Identifica quais corpos colidiram
			b2Fixture* body2 = point->fixtureB;
			int tag1 = body1->GetUserData().pointer;  //Recebe o indíce daquele corpo
			int tag2 = body2->GetUserData().pointer;

			//RESET DE PULOS (PLAYER->2 & PLATAFORMA->1)
			if ((tag1 == 1 && tag2 == 2) || (tag2 == 1 && tag1 == 2))
			{
				pulos = 0;
			}
			

				//MORTE NOS ESPINHOS (PLAYER->2 & ESPINHOS->3)
		    if ((tag1 == 2 && tag2 == 3) || (tag2 == 3 && tag1 == 2))
			{
				player->SetTransform(b2Vec2(inicialX, inicialY), 0.0f);
				float velLinX = player->GetLinearVelocity().x;
				float velLinY = player->GetLinearVelocity().y;

				b2Vec2 playerVec;
				playerVec = b2Vec2(-velLinX, -velLinY);
				playerVec.operator*=(50.0f * 1.0f);
				
				player->ApplyLinearImpulseToCenter(playerVec, true);
				
				
			}

			//VITÓRIA (PLAYER-> 2 & BANDEIRA->4)
			if ((tag1 == 2 && tag2 == 4) || (tag2 == 4 && tag1 == 2))
			{
				glfwSetWindowShouldClose(g_mainWindow, GL_TRUE);
			}
		}

	}
	

	void cameraFollow()
	{
		g_camera.m_center.Set(player->GetPosition().x, player->GetPosition().y+10.0f);
		g_camera.m_zoom = 2.f;

	}

	b2Vec2 getVetor(float escala = 1)
	{
		b2Vec2 force;


		float difX = m_mouseWorld.x - player->GetPosition().x ;
		float difY = m_mouseWorld.y - player->GetPosition().y ;


		float vetorX = difX / sqrt(difX * difX + difY * difY);
		float vetorY = difY / sqrt(difX * difX + difY * difY);
	

		force = b2Vec2(-vetorX, -vetorY);
		force.operator*=(intensidade * escala);

		return force;
	}

	void refreshVetor()
	{
		if (vetor->GetFixtureList())
		{
			vetor->DestroyFixture(vetor->GetFixtureList());
		}
		float difX = m_mouseWorld.x - player->GetPosition().x;
		float difY = m_mouseWorld.y - player->GetPosition().y;

		float vetorX = difX / sqrt(difX * difX + difY * difY);
		float vetorY = difY / sqrt(difX * difX + difY * difY);
		//b2Vec2 ponto = b2Vec2(m_mouseWorld.x , m_mouseWorld.y );

		b2Vec2 ponto = b2Vec2(-vetorX , -vetorY);
		b2EdgeShape staticLine;
		staticLine.m_vertex1 = player->GetPosition();
		staticLine.m_vertex2 = ponto + player->GetPosition() + getVetor(0.01f);

		vetor->CreateFixture(&staticLine, 0);

	}
	
	float degreesToRadians(float angle) {
		float radians = angle * b2_pi / 180.0f;

		return radians;
	};

	
	

};

//Aqui fazemos o registro do novo teste 
static int testIndex = RegisterTest("Examples", "GA_teste", GA_teste::Create);