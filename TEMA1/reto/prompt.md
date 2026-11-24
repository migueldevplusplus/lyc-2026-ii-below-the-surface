# Prompt usado con el LLM para generar lexer_llm.py

Escribe en Python un analizador lexico (funcion `lexer(codigo)` que devuelve una lista de tuplas
(tipo, texto)) para este mini-lenguaje, usando regex. Ignora espacios.
- PALABRA_RESERVADA: if, else, while, return
- IDENTIFICADOR: [A-Za-z_][A-Za-z0-9_]*
- NUMERO: entero o decimal (3, 3.14)
- CADENA: texto entre comillas dobles, sin saltos de linea
- OPERADOR: == != <= >= + - * / = < >
- DELIMITADOR: ( ) { } ; ,
- COMENTARIO: // hasta fin de linea
- ERROR: cualquier otro caracter
Aplica la regla de la coincidencia mas larga.
