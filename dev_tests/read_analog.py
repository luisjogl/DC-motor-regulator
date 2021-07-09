#!/usr/bin/python

import spidev

import time

import os

# SPI bus

spi = spidev.SpiDev()

spi.open(0,0)

# Funcion para leer los datos SPI desde el chip MCP3008

# El canal debe ser un entero 0-7

def ReadChannel(channel):

	adc = spi.xfer2([1,(8+channel)<<4,0])

	data = ((adc[1])<<8)+adc[2]

	return data

# Funcion para convertir datos a nivel de voltaje,

# Redondeado al numero especificado de decimales.

def ConvertVolts(data,places):

	volts = (data * 5.0) / float(1023)

# si la tension de referencia es 3,3V, la linea volts = (data *3.3)/float(1023)

	volts = round(volts,places)

	return volts

# Definir los canales del sensor

pot = 4

# Definir el retardo entre lecturas

delay = 3

while True:

# Leer los datos del del potenciometro

	adc = ReadChannel(pot)

	pot_volts = ConvertVolts(adc,2)

# Imprimir resultados

	print ("--------------------------------------------")

	print ("Lectura ADC: ", adc)

	print("Voltaje: {}V".format(pot_volts))

	# Esperar antes de repetir el bucle

	time.sleep(delay)
