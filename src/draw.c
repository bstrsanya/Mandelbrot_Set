#include "common.h"

void draw (void (*GetPointFunc) (int* vertex_array, Param* param))
{
    sfRenderWindow* window;
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, DEEP_COLOR };
    window           = sfRenderWindow_create (mode, NAME_WINDOW, sfClose, NULL);

    sfSprite* sprite   = sfSprite_create ();
    sfTexture* texture = sfTexture_create (WINDOW_WIDTH, WINDOW_HEIGHT);

    sfUint32* pixels = (sfUint32*) calloc (WINDOW_WIDTH * WINDOW_HEIGHT, sizeof (sfUint32));

    Param param = { .offsetX = INIT_SHIFT, .offsetY = INIT_SHIFT, .scale = INIT_SCALE };

    sfFont* font = sfFont_createFromFile (WAY_FONT);
    sfText* text = sfText_create ();
    sfText_setFont (text, font);
    sfText_setCharacterSize (text, SIZE_BUF_TEXT);
    sfText_setColor (text, sfBlue);

    uint32_t* array_color = (uint32_t*) calloc (MAX_NUM_ITER + 1, sizeof (uint32_t));
    GetColor (array_color);

    uint64_t t1 = get_rdtsc ();
    uint64_t t2 = 0;

    while (sfRenderWindow_isOpen (window))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent (window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close (window);

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

        float fps = 1.0f / ((float) (t2 - t1) / cpu_freq);
        t1        = t2;

        char fpsString[SIZE_BUF_TEXT];
        sprintf (fpsString, "FPS: %.1f", fps);
        sfText_setString (text, fpsString);

        sfText_setPosition (text, (sfVector2f) { 0.0f, 0.0f });

        sfRenderWindow_clear (window, sfBlack);

        int* cur_value_color = (int*) calloc (WINDOW_HEIGHT * WINDOW_WIDTH, sizeof (int));
        GetPointFunc (cur_value_color, &param);

        for (int y = 0; y < WINDOW_HEIGHT; y++)
        {
            uint32_t* cur_pixel = &pixels[y * WINDOW_WIDTH];

            for (int x = 0; x < WINDOW_WIDTH; x++)
            {
                cur_pixel[x] = array_color[cur_value_color[x * WINDOW_HEIGHT + y]];
            }
        }

        sfTexture_updateFromPixels (texture, (const sfUint8*) pixels, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
        free (cur_value_color);

        sfSprite_setTexture (sprite, texture, sfTrue);
        sfRenderWindow_drawSprite (window, sprite, NULL);

        sfRenderWindow_drawText (window, text, NULL);

        sfRenderWindow_display (window);
    }

    sfFont_destroy (font);
    sfText_destroy (text);
    sfSprite_destroy (sprite);
    sfTexture_destroy (texture);
    sfRenderWindow_destroy (window);
    free (array_color);
    free (pixels);
}

void GetColor (uint32_t* array)
{
    uint32_t red   = 0;
    uint32_t green = 0;
    uint32_t blue  = 0;

    array[MAX_NUM_ITER] = (OPAQUE << SHIFT_24) | (red) | (green << SHIFT_8) | (blue << SHIFT_16);

    for (int t = 0; t < MAX_NUM_ITER; t++)
    {
        red      = (unsigned char) (((int) round ((SHIFT_COLOR + RED_A * sin (RED_B * (float) t + RED_C)) * MAX_COLOR)) % NORMAL_CHAR);
        green    = (unsigned char) (((int) round ((SHIFT_COLOR + GREEN_A * sin (GREEN_B * (float) t + GREEN_C)) * MAX_COLOR)) % NORMAL_CHAR);
        blue     = (unsigned char) (((int) round ((SHIFT_COLOR + BLUE_A * sin (BLUE_B * (float) t + BLUE_C)) * MAX_COLOR)) % NORMAL_CHAR);
        array[t] = (OPAQUE << SHIFT_24) | (red) | (green << SHIFT_8) | (blue << SHIFT_16);
    }
}