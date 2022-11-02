#include <list>
#include <random>
#include <string>
#include "Particle.h"
class Firework :public Particle
{


public:
	//virtual Particle* clone() const;
	std::list<Particle*> explode();
	Firework(Vector3 p, Vector3 initialSpeed, Vector3 a, float m, float d, RenderItem* ri, float sizePart, int nDiv, Vector4 c = { 0.1,0.8,0.2,1 }, double tA = 5, int nGen = 5, string cV = "nada");
private:
	std::list<std::shared_ptr<GeneradorSimple> >gens_; //metodo que da generadores
	Vector3 actionSpace;
	Vector4 color;
	int nPart;
	int nGeneraciones;
	int sizeF;
	int tAlive;
	string circulo;


};
Firework::Firework(Vector3 p, Vector3 initialSpeed, Vector3 a, float m, float d, RenderItem* ri, float sizePart, int nDiv, Vector4 c, double tA, int nP, string cV)
	: Particle(p, initialSpeed, a, m, d, ri, { 1000,1000,1000 }, c, tA)
{
	actionSpace = { 1000,1000,1000 }; //Este espacio de accion es para 
	nGeneraciones = nDiv;
	nPart = nP;
	sizeF = sizePart;
	tAlive = tA;
	circulo = cV;

	//color = c;


}
std::list<Particle*> Firework::explode()
{
	random_device rand;
	default_random_engine generator = default_random_engine(rand());
	normal_distribution <double> normal{ 10,15 };
	std::list<Particle*>listExp;
	int nDivisiones = 20;

	float angle = 200;
	bool change = false;
	float radius = 2;
	if (nGeneraciones > 0) {
		for (int i = 0; i < nPart; i++)
		{
			Vector3 dir2{ (float)normal(generator),(float)normal(generator),(float)normal(generator) };
			Vector4 colorRand{ (float)normal(generator),(float)normal(generator),(float)normal(generator),1 };

			int offset = 1.25;
			int offsetCircle = 5;
			Vector3 pos = getPosition() * offset;
			if (circulo == "circulo")
			{

				dir2 = { float(physx::PxCos(angle) * radius),0, float(physx::PxSin(angle) * radius) };
				dir2 *= offsetCircle;
				angle += 360 / nPart;
			}
			else if (circulo == "circulo3d")
			{
				dir2 = { float(physx::PxCos(angle) * radius),float(physx::PxSin(angle) * radius), float(physx::PxSin(angle) * radius) };
				dir2 *= offsetCircle;
				angle += 360 / nPart;
			}
			else if (circulo == "explosion")
			{
				if (!change)
				{
					dir2.x = 16 * i * (physx::PxSin(angle) * physx::PxSin(angle) * physx::PxSin(angle)) * -1;
					dir2.y =
						20 * i * physx::PxCos(angle) +
						2 + i * physx::PxSin(angle) +
						1 * i * physx::PxCos(angle);
					dir2.y *= -1;
					dir2.z = (16 / (i + 1) * i * (physx::PxCos(angle))) * -1;
					change = true;
				}
				else {
					dir2.x = 16 * i * (physx::PxSin(angle) * physx::PxSin(angle) * physx::PxSin(angle));
					dir2.y =
						20 * i * physx::PxCos(angle) +
						2 + i * physx::PxSin(angle) +
						1 * i * physx::PxCos(angle);

					dir2.z = 10 * i * (physx::PxCos(angle));
					change = false;
				}
				angle++;
			}
			else if (circulo == "lineas")
			{
				if (!change)
				{
					dir2.x = 16 * i * (physx::PxSin(angle) * physx::PxSin(angle) * physx::PxSin(angle)) * -1;
					dir2.y =
						20 * i * physx::PxCos(angle) +
						2 + i * physx::PxSin(angle) +
						1 * i * physx::PxCos(angle);
					dir2.y *= -1;
					dir2.z = (16 / (i + 1) * i * (physx::PxCos(angle))) * -1;
					change = true;
				}
				else {
					dir2.x = 16 * i * (physx::PxSin(angle) * physx::PxSin(angle) * physx::PxSin(angle));
					dir2.y =
						20 * i * physx::PxCos(angle) +
						2 + i * physx::PxSin(angle) +
						1 * i * physx::PxCos(angle);

					dir2.z = 10 * i * (physx::PxCos(angle));
					change = false;
				}
			}
			//Aprovechar la posicion de firework para que parta mas o menos desde ahi
			Firework* p = new Firework(pos, dir2, { 0.5,0.5,0.5 }, 1, 0.99, new RenderItem(CreateShape(PxSphere(sizeF)), Vector4(1, 0, 1, 1)), sizeF, nGeneraciones - 1, colorRand, tAlive / 1.25, nPart, circulo);
			listExp.push_back(p);


		}

	}
	return listExp;
}