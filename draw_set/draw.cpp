#include "common.h"

void draw (void (*GetPoint_func)(int* vertex_array, Param* param))
{
    sfRenderWindow* window;
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, DEEP_COLOR};
    window = sfRenderWindow_create(mode, NAME_WINDOW, sfClose, NULL);

    sfVertexArray* vertex_array = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(vertex_array, sfPoints);

    Param param = {.offsetX = INIT_SHIFT, .offsetY = INIT_SHIFT, .scale = INIT_SCALE};

    sfFont* font = sfFont_createFromFile(WAY_FONT);
    sfText* text = sfText_create();
    sfText_setFont(text, font); 
    sfText_setCharacterSize(text, SIZE_BUF_TEXT); 
    sfText_setColor(text, sfBlue);  

    Color* array_color = (Color*) calloc (MAX_NUM_ITER + 1, sizeof (Color));
    GetColor (array_color);

    uint64_t t1 = get_rdtsc();
    uint64_t t2 = 0;

    while (sfRenderWindow_isOpen(window)) 
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) 
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            
            if (event.type == sfEvtKeyPressed) 
            {
                switch ((int) event.key.code)
                {
                    case sfKeyDown:
                        param.offsetY += param.scale * COEF_SHIFT;
                        break;

                    case sfKeyLeft:
                        param.offsetX += param.scale * COEF_SHIFT;
                        break;
                    
                    case sfKeyRight:
                        param.offsetX -= param.scale * COEF_SHIFT;
                        break;

                    case sfKeyUp:
                        param.offsetY -= param.scale * COEF_SHIFT;
                        break;

                    case sfKeyA:
                        param.scale *= ZOOM_IN;
                        break;
                    
                    case sfKeyS:
                        param.scale *= ZOOM_OUT;
                        break;

                    case sfKeyH:
                        param.scale   = INIT_SCALE;
                        param.offsetX = INIT_SHIFT;
                        param.offsetY = INIT_SHIFT;

                    default:
                        break;
                }
            }
        }

        t2 = get_rdtsc ();
        
        float fps = 1.0f / ((float)(t2 - t1) / cpu_freq);
        t1 = t2;

        char fpsString[SIZE_BUF_TEXT];
        sprintf(fpsString, "FPS: %.1f", fps);
        sfText_setString(text, fpsString);

        sfText_setPosition(text, (sfVector2f){0.0f, 0.0f});

        sfRenderWindow_clear(window, sfBlack);

        sfVertexArray_clear(vertex_array);

        int* cur_value_color = (int*) calloc (WINDOW_HEIGHT * WINDOW_WIDTH, sizeof (int));
        GetPoint_func (cur_value_color, &param);

        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            for (int y = 0; y < WINDOW_HEIGHT; y++)
            {
                Color value_color = array_color[cur_value_color[x * WINDOW_HEIGHT + y]];

                sfVertex vertex = 
                {
                    .position = {(float) x, (float) y},
                    .color = sfColor_fromRGB(value_color.red, 
                                             value_color.green, 
                                             value_color.blue)
                };
                sfVertexArray_append(vertex_array, vertex);
            }
        }
        free (cur_value_color);


        sfRenderWindow_drawVertexArray(window, vertex_array, NULL);
        sfRenderWindow_drawText(window, text, NULL);

        sfRenderWindow_display(window);
    }

    sfFont_destroy(font);
    sfText_destroy(text);
    sfVertexArray_destroy(vertex_array);
    sfRenderWindow_destroy(window);
    free(array_color);
}