#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // Para funciones relacionadas con direcciones IP y sockets
#include <sys/socket.h>
#include <sys/types.h>

#define PUERTO_SERVIDOR 9999   // Puerto del servidor
#define TAMAÑO_BUFFER 1024    // Tamaño del búfer de recepción

// Función para conectarse al servidor y recibir el archivo
void recibirArchivo(const char *direccionIP) {
    int socket_cliente;
    struct sockaddr_in servidor_addr;
    char buffer[TAMAÑO_BUFFER]; // Tamaño bloques datos

    // ----- 1 Crear SOCKET del Cliente (punto final de comunicación para comunicación)-----
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == -1) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configuración de la dirección del servidor
    servidor_addr.sin_family = AF_INET; // Trabajaremos con IPv4
    servidor_addr.sin_port = htons(PUERTO_SERVIDOR); // Traducción a htons (entienda la red el puerto)
    if (inet_pton(AF_INET, direccionIP, &servidor_addr.sin_addr) <= 0) { // IPv4 en formato para que entienda la red
        perror("Dirección IP inválida o no soportada");
        close(socket_cliente); // Cierra socket cliente
        exit(EXIT_FAILURE);
    }

    // ----- 3 CONNECT (Cliente conecta con el servicio) -----
    printf("Conectando al servidor %s en el puerto %d...\n", direccionIP, PUERTO_SERVIDOR); // Puntero que direcciona a dir. IP y puerto
    if (connect(socket_cliente, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Error al conectar con el servidor");
        close(socket_cliente); // Cierra socket cliente
        exit(EXIT_FAILURE);
    }
    printf("Conexión establecida. Recibiendo datos...\n");

    // ----- 4 READ (Recibir datos del servidor) -----
    int bytes_leidos;
    while ((bytes_leidos = recv(socket_cliente, buffer, TAMAÑO_BUFFER - 1, 0)) > 0) {
        buffer[bytes_leidos] = '\0'; // Añadir caracter nulo al final de los datos recibidos (en C para indicar fin string)
        printf("%s", buffer);       // Mostrar los datos en la terminal %s string, contenido en el buffer
    }

    if (bytes_leidos == -1) {
        perror("Error al recibir datos");
    }

    // ----- 5 CLOSE (Cerrar el socket) -----
    printf("\nTransferencia completa. Cerrando conexión...\n");
    close(socket_cliente); // Cierra socket cliente
}


// ----- MAIN -----
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Se usa de esta manera: %s <DIRECCIÓN_IP_SERVIDOR>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    recibirArchivo(argv[1]); // Llamar a la función con la IP del servidor
    return 0;
}
