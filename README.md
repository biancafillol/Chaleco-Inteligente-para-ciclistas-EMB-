# Chaleco Inteligente para Ciclistas

Este proyecto corresponde al desarrollo de un **chaleco inteligente** orientado a mejorar la seguridad de ciclistas en entornos urbanos y rurales. Fue realizado como parte de una materia de la carrera de Ingeniería en Computación en la **Universidad Nacional de Río Negro (UNRN)**.

## Objetivo

Diseñar un prototipo funcional de chaleco que:
- Mejore la **visibilidad del ciclista** mediante señales luminosas.
- Permita señalizar **giros, freno y emergencia** usando botones.

## Componentes principales

- ESP32 (uno para manejar la botonera y otro para la matriz LED)
- Módulo LED MAX7219 (4 matrices 8x8 en cascada)
- Botonera (4 botones para señalizar acciones)
- Batería externa (power bank)

## Archivos disponibles

En este repositorio encontrarás los siguientes códigos:

- `conexionBotones.ino`  
  Código encargado de detectar el estado de los botones conectados al chaleco y activar las señales correspondientes.

- `conexionLEDs.ino`  
  Código encargado de controlar las matrices LED con animaciones de giro a izquierda, derecha, freno y emergencia.

>  Los archivos están separados para facilitar la prueba y depuración de cada módulo de forma independiente.

## Estado del proyecto

- Animaciones LED funcionales (giro, freno).
- Botonera conectada correctamente y detectando acciones.
- Pruebas realizadas con resultados satisfactorios.

## Futuras mejoras

- Integrar GPS con envío de ubicación en caso de emergencia.
- Añadir una interfaz web o app para seguimiento en tiempo real.

## Integrantes del proyecto

**Bianca Fillol, Agustina Bover y Manuel Bernabei**  
Estudiantes de Ingeniería en Computación - UNRN  

---

¡Gracias por visitar este proyecto!

![Badge](https://bit.ly/icom-badge)
