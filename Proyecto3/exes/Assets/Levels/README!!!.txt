Instrucciones para escribir los niveles en .json: (perdon por el textaco :) )

Habra un archivo .json por cada nivel que haya en el juego, y en el se escribiran los nombres de las entidades
que se quiere que aparezcan. En Prefabs.json estaran definidos los componentes que queremos que tengan las entidades
del otro archivo. Por ejemplo: en Example00.json queremos que aparezcan un Caracol y un Limon, y en Prefabs.json
estaran definidos los componentes que habra que añadir a una entidad para que sea un Caracol o un Limon.

Si ademas queremos cambiar algun parametro que venga por defecto en algun componente en Prefabs.json, habra que añadirselo
a Example00.json (hay que poner TODOS los parametros en orden en el que los reciba la constructora, y los que queremos que 
se mantengan con sus valores por defecto los dejaremos con "-").

Lo mismo ocurrira si queremos tener listeners y emitters entre los componentes de la entidad.
Abajo del todo habra que poner las entidades que queremos que sean emitters/listeners de otras, si es que hubiera.

Ejemplo de como se escribirian:


Example00.json:
{
    "GameObjects": [
        {
            "Name": "Caracol",               // entidad tipo Caracol (definida en Prefabs)
 	    "GOParameters":["10", "-", "-"], // parametros de la constructora que se quieren cambiar
            "Custom": [                      // parametros que se quieren cambiar de algun componente
                {
                    "Name": "Render",        // parametros que queremos cambiar del componente "Render"
                    "Parameters": [
                        "2",                 // parametro que cambiamos
                        "-"                  // parametro que se mantiene por defecto
                    ]
                }
            ],
            "ComponentMessages": [           // queremos que el componente "Render" le envie mensajes al componente "Update" 
                {
                    "Emitter": "Render",
                    "Listener": "Update"
                }
            ]
        },
        {
            "Name": "Limon"                  // entidad tipo Limon, con todo por defecto
        }
    ],

    "GameObjectMessages": [                  // queremos que el Caracol le envie mensajes a Limon
        {
            "Emitter": "Caracol",
            "Listener": "Limon"
        }
    ]
}


Prefabs.json:
{
    "Caracol":[                              // por defecto, la entidad Caracol tendra componente "Render", con X parametros y "Update", con Y parametros
        {
            "GOParameters":["0", "0", "0"]   // parametros por defecto de la constructora (si no recibe ninguno no poner este campo)
        },

        {
            "Name" : "Render",
            "Parameters":
            [
                "50", "20"
            ]
        },
        {
            "Name" : "Update",
            "Parameters":
            [
                "2"
            ]
        }
    ],
    "Limon":[                             // lo mismo pasa con Limon
        {
            "GOParameters":["0", "0", "0"]
        },

        {
            "Name" : "Render",
            "Parameters":
            [
                "20", "20"
            ]
        }
    ],

    // Para generar mapas o suelos (como quieras llamarlo), hay que escribir en <nombre del nivel
    // que quieras (el mismo que el .json)>.txt las filas y las columnas que va a tener, seguido
    // de una matriz de numeros de esas dimensiones en la que 0 significara baldosa normal, 1 baldosa
    // que se cae y # que no hay baldosa en esa posicion. El suelo aparecera con su centro en el (0, 0, 0)
    // y los tipos de suelo estaran definidos en Prefabs.json:
    "Tile":[
        {
            "GOParameters":["0", "0", "0"]
        },
        {
            "Name" : "Render",
            "Parameters":
            [
                "10", "10"
            ]
        }
    ],
    "FallingTile":[
        {
            "GOParameters":["0", "0", "0"]
        },
        {
            "Name" : "Render",
            "Parameters":
            [
                "30", "10"
            ]
        }
    ]
}

Example00.txt (ejemplo de como escribir un mapa)
3 3
00#
000
#10
            [
                "30", "10"
            ]
        }
    ]
}