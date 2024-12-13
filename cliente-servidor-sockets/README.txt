----- Proyecto de Comunicación en Red - Cliente-Servidor TCP -----

- Descripción: -
Este proyecto implementa una simple aplicación cliente-servidor usando sockets TCP/IP para realizar transferencias de archivos
a través de la red. El servidor espera y escucha a conexiones entrantes en un puerto especificado y envía un archivo al cliente
una vez establecida la conexión.



- Estructura del Proyecto -
1) cliente.c: Implementa el cliente TCP que se conecta al serrvidor, recibe el archivo y muestra su contenido en la consola.
2) servidor.c: Implementa el servidor TCP que escucha conexiones, acepta clientes y transfiere el archivo especificado.
3) google.gtml: Un archivo de ejemplo utilizando para la transferencia. Contiene el código de una página web simple.



- Cómo Ejecutar en linux/ubuntu -
1) Compilar los programas:
        gcc -o cliente cliente.c
        gcc -o servidor servidor.c
2) Ejecutar el servidor:
        ./servidor
3) Ejecutar el cliente (en otra terminal)
        ./cliente <DIRECCIÓN_IP_SERVIDOR>     * Reemplaza <DIRECCIÓN_IP_SERVIDOR> con la dirección IP del servidor.



- Cómo Ejecutar en Windows-
Es básicamente el mismo proceso, pero deberás tener en caso de no usar un virtual box ubuntu. Es muy importante
ejecutar en dos terminales diferentes pero en el mismo contenedor.



- Requisitos -
1) Sistema operativo Linux o compatible con sockets POSIX, o Docker en Windows.
2) Compilador GCC.



- Notas -
1) El servidor utiliza el puerto 9999 de manera predeterminada.
2) Asegúrate de que el archivo google.html esté en el mismo directorio que el ejecutable del servidor.



- Autor -
Mauro Valls Vidal - 2º Ingeniería en Inteligencia Artificial (Curso 2024/2025).
