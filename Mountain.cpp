#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

std::atomic<int> sync;
//std::atomic<int> assentos;

int assentos;
int _next;
int turn[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


bool parquinho_aberto;
bool esperando;
bool chegou;
bool saiu;

void Passageiro(int id) {

	int i = id;
	while (parquinho_aberto)
	{
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		printf("Passageiro %d dando um role no parque\n", i);
		turn[i] = std::atomic_fetch_add(&sync, 1);

		while (_next != turn[i]); //protocolo de entrada

		printf("Passageiro %d esperando o carrinho\n", i);
		while (!esperando); //protocolo de entrada
		while (assentos > 5); // protocolo de entrada
		++assentos;
		++_next;
		printf("Passageiro %d subiu no carrinho\n", i);
		while (!saiu);
		while (!chegou);

		printf("Passageiro %d desceu do carrinho\n", i);

		--assentos;
	}
}

void Carro() {

	int voltas = 0;


	while (voltas < 10) {
		printf("O carrinho chegou!!\n");
		chegou = true;
		saiu = false;

		printf("Esperando os passageiros descerem...\n");
		while (assentos != 0);
		esperando = true;
		printf("Esperando os passageiros subirem...\n");
		while (assentos != 5);
		esperando = false;
		chegou = false;
		//Esse atraso serve para permitir que todas as threads avisem
		//que subiram no carrinho
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
		printf("O carrinho esta dando uma volta.......\n");
		saiu = true;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		voltas++;
	}
}

int main()
{
	thread pessoas[10];

	_next = 0;
	parquinho_aberto = true;
	chegou = true;
	assentos = 0;

	thread carrinho = std::thread(Carro);

	for (int i = 0; i < 10; i++) {
		pessoas[i] = std::thread(Passageiro, i);
	}
	carrinho.join();
	parquinho_aberto = false;

	cout << "pronto" << endl;
	cin;

	return 0;
}
