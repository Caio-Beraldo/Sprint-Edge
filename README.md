# Sprint3-Edge
Descrição do Projeto

Este projeto tem como objetivo desenvolver um sistema de monitoramento em tempo real da ocupação de diferentes salas, utilizando sensores conectados a um microcontrolador ESP32. Os dados são coletados localmente, exibidos em um display OLED e enviados para a nuvem via Wi-Fi para visualização remota por meio da plataforma Taggio.

Objetivos
-Medir o nível de ocupação com sensores.
-Apresentar visualmente o status de ocupação em um display OLED.
-Enviar os dados para a nuvem via ESP32 utilizando Wi-Fi.
-Visualizar os dados em tempo real através de um dashboard (web/app) na plataforma Taggio.

Descrição dos Componentes
ESP32:	Microcontrolador com suporte Wi-Fi e Bluetooth, responsável pela leitura dos sensores, exibição no display e envio dos dados.
Sensores (A0-A2):	Sensores que capturam a presença ou movimento em cada sala.
Display OLED 128x64:	Tela usada para exibir o status local de ocupação de cada sala.

Sobre o diagrama
Camada IoT: Sensores (A0, A1, A2) conectados ao ESP32 capturam os dados de ocupação de cada sala. O ESP32 lê os valores analógicos, classifica o nível de ocupação e exibe essa informação no display OLED.
Camada Back-End: O ESP32 se comunica com a plataforma Taggio, enviando os dados dos sensores via protocolo MQTT ou HTTP.
Camada Aplicação: A interface de visualização consiste em um dashboard web ou aplicativo mobile, onde os dados são visualizados de forma amigável por administradores ou usuários.

Integrantes:
Caio Costa Beraldo - RM560775
Ícaro Henrique de Souza Calixto - RM560278
Victor Kenzo Mikado - RM560057
Pietro Brandalise De Andrade - RM560142
