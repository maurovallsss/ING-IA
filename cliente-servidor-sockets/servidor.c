#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // Funciones IP
#include <sys/socket.h>
#include <sys/types.h>

#define PUERTO 9999     // Puerto del servidor (Nu. ID. serv. app)
#define ARCHIVO_ENVIAR "Google.html" // Archivo que será enviado
#define LIMITE_COLA 10       // Número máximo de conexiones en espera
#define TAMAÑO_PAQUETES 1024      // Tamaño del bloque de datos a enviar

// ----- 6 Tratar Petición (servidor procesa la solicitud) ------    *Función para manejar el envío del archivo al cliente
void transferirArchivo(int cliente_fd) {
    FILE *archivo = fopen(ARCHIVO_ENVIAR, "r"); // Abrir el archivo en modo lectura
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo para enviar");
        close(cliente_fd); //Cierra socket de cliente 
        return;
    }

    printf("Enviando archivo al cliente...\n");
    char buffer[TAMAÑO_PAQUETES]; //Almacena bloques datos
    size_t leidos;

    // ----- 7 SEND (El archivo se envía al cliente en bloques de datos) ------   *Leer y enviar datos del archivo al cliente
    while ((leidos = fread(buffer, 1, TAMAÑO_PAQUETES, archivo)) > 0) {
        if (send(cliente_fd, buffer, leidos, 0) == -1) {
            perror("Error al enviar datos al cliente");
            break;
        }
    }
    // ----- 8 CLOSE (El socket asociado a la conexión con el cliente se cierra al finalizar la transferencia) ------ 
    fclose(archivo);
    close(cliente_fd); //Cierra socket de cliente 
    printf("Transferencia completada y conexión cerrada.\n");
}

int main() {
    int servidor_fd, cliente_fd;
    struct sockaddr_in direccion_servidor, direccion_cliente;
    socklen_t tam_cliente = sizeof(direccion_cliente); // Reserva espacio para el tamaño de la dirección

    // ----- 1 SOCKET (Extremo de comunicación envio/recibo de paquetes) ------
    servidor_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
    if (servidor_fd == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    direccion_servidor.sin_family = AF_INET; //IPv4
    direccion_servidor.sin_port = htons(PUERTO); // Transf. puerto a que entienda red
    direccion_servidor.sin_addr.s_addr = INADDR_ANY; // WiFi, Ethernet (interfaz) host

    // ----- 2 BIND (Asociar el socket a la dirección y puerto) ------
    if (bind(servidor_fd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) == -1) {
        perror("Error al asociar el socket al puerto");
        close(servidor_fd); //Libera socket creado
        exit(EXIT_FAILURE);
    }

    //  ----- 3 LISTEN (El servidor comienza a escuchar conexiones en el puerto asociado) ------
    if (listen(servidor_fd, LIMITE_COLA) == -1) {
        perror("Error al poner el socket en modo escucha");
        close(servidor_fd); //Libera socket creado
        exit(EXIT_FAILURE);
    }

    printf("Servidor en funcionamiento, esperando conexiones en el puerto %d...\n", PUERTO);

    // Bucle para aceptar y manejar conexiones
    while (1) {
        // ----- 4 Accept (El servidor espera una conexión entrante y la acepta) ------
        cliente_fd = accept(servidor_fd, (struct sockaddr *)&direccion_cliente, &tam_cliente); // Nuevo socket exclusivo para cliente
        if (cliente_fd == -1) {
            perror("Error al aceptar conexión");
            continue;
        }

        printf("Conexión establecida con %s:%d\n",
               inet_ntoa(direccion_cliente.sin_addr),
               ntohs(direccion_cliente.sin_port));

        // Crear un proceso hijo para manejar la transferencia
        if (fork() == 0) {
            close(servidor_fd); // El proceso hijo no necesita el socket del servidor
            transferirArchivo(cliente_fd); // Transferir el archivo al cliente
            exit(EXIT_SUCCESS); // Terminar el proceso hijo
        }

        close(cliente_fd); // El proceso padre cierra el socket del cliente
    }
    // ----- 8 CLOSE (El socket se cierra al finalizar el programa) ------ 
    close(servidor_fd);
    return 0;
}
