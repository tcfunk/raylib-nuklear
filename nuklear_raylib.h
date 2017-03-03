//
// Created by Tyler Funk on 3/3/17.
//

#ifndef NUKLEAR_RAYLIB_H
#define NUKLEAR_RAYLIB_H

#include <math.h>
#include "raylib.h"

static struct nk_context ctx;
static font_size = 14;

static float prev_mouse_x;
static float prev_mouse_y;

typedef struct NKSpriteFont NKSpriteFont;
struct NKSpriteFont {
    struct nk_user_font nk;
    int height;
    int width;
    SpriteFont *font;
};

NK_API struct nk_context* nk_raylib_init(NKSpriteFont* font, int screenWidth, int screenHeight);
NK_API NKSpriteFont* nk_raylib_font_create(const char* file_name);
NK_API void nk_raylib_free();


// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


NK_API NKSpriteFont*
nk_raylib_font_create(const char* file_name)
{
    SpriteFont sprite_font;
    if (strcmp(file_name, "default") == 0)
    {
        sprite_font = GetDefaultFont();
    } else {
        sprite_font = LoadSpriteFont(file_name);
    }

    NKSpriteFont* font = (NKSpriteFont*)calloc(1, sizeof(NKSpriteFont));

    font->font = &sprite_font;
    font->height = sprite_font.charRecs->height;
    font->width = sprite_font.charRecs->width;
    return font;
}

static float
nk_raylib_font_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    NKSpriteFont* font = (NKSpriteFont*)handle.ptr;

    if (!font || !text) {
        return 0;
    }

    char strcpy[len+1];
    strncpy((char*)&strcpy, text, len);
    strcpy[len] = '\0';

    SpriteFont* sprite_font = font->font;

//    Vector2 measurements = MeasureTextEx(*sprite_font, text, sprite_font->size, 1);
//    return measurements.x;
    return MeasureText(text, 14);
}


NK_API struct nk_context*
nk_raylib_init(NKSpriteFont* font, int screenWidth, int screenHeight)
{
    struct nk_user_font *nk_font = &font->nk;
    nk_font->userdata = nk_handle_ptr(font);
    nk_font->height = (float) font->height;
    nk_font->width = nk_raylib_font_get_text_width;

    nk_init_default(&ctx, nk_font);
    return &ctx;
}


static Color
nk_color_to_raylib_color(struct nk_color color)
{
    Color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}


static void
nk_raylib_render()
{
    const struct nk_command *cmd;

    nk_foreach(cmd, &ctx)
    {
        Color color;
        switch (cmd->type)
        {
            case NK_COMMAND_NOP: break;

            case NK_COMMAND_SCISSOR:
            {
//                @TODO
            } break;

            case NK_COMMAND_LINE:
            {
                const struct nk_command_line *l = (const struct nk_command_line *)cmd;
                color = nk_color_to_raylib_color(l->color);
                DrawLine(l->begin.x, l->begin.y, l->end.x, l->end.y, color);
            } break;

            case NK_COMMAND_RECT:
            {
                const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
                color = nk_color_to_raylib_color(r->color);
                DrawRectangleLines(r->x, r->y, r->w, r->h, color);
            } break;

            case NK_COMMAND_RECT_FILLED:
            {
                const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
                color = nk_color_to_raylib_color(r->color);
                DrawRectangle(r->x, r->y, r->w, r->h, color);
            } break;

            case NK_COMMAND_CIRCLE:
            {
                const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
                color = nk_color_to_raylib_color(c->color);
                float r = fmin(c->w, c->h);
                DrawCircleLines(c->x, c->y, r, color);
            } break;

            case NK_COMMAND_CIRCLE_FILLED:
            {
                const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
                color = nk_color_to_raylib_color(c->color);
                float r = fmin(c->w, c->h);
                DrawCircle(c->x, c->y, r, color);
            } break;

            case NK_COMMAND_TRIANGLE:
            {
                const struct nk_command_triangle *t = (const struct nk_command_triangle*)cmd;
                color = nk_color_to_raylib_color(t->color);
                DrawTriangleLines((Vector2){t->a.x, t->a.y}, (Vector2){t->b.x, t->b.y}, (Vector2){t->c.x, t->c.y}, color);
            } break;

            case NK_COMMAND_TRIANGLE_FILLED:
            {
                const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled*)cmd;
                color = nk_color_to_raylib_color(t->color);
                DrawTriangle((Vector2){t->a.x, t->a.y}, (Vector2){t->b.x, t->b.y}, (Vector2){t->c.x, t->c.y}, color);
            } break;

            case NK_COMMAND_POLYGON:
            {
                const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
                color = nk_color_to_raylib_color(p->color);
                Vector2 points[p->point_count];

                for (int i = 0; i < p->point_count; i++) {
                    points[i] = (Vector2){p->points[i].x, p->points[i].y};
                }

                DrawPolyEx(points, p->point_count, color);
            } break;

            case NK_COMMAND_POLYGON_FILLED:
            {
                const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled*)cmd;
                color = nk_color_to_raylib_color(p->color);
                Vector2 points[p->point_count];

                for (int i = 0; i < p->point_count; i++) {
                    points[i] = (Vector2){p->points[i].x, p->points[i].y};
                }

                DrawPolyEx(points, p->point_count, color);
            } break;

            case NK_COMMAND_TEXT:
            {
                const struct nk_command_text *t = (const struct nk_command_text*)cmd;
                color = nk_color_to_raylib_color(t->foreground);
                NKSpriteFont* font = (NKSpriteFont*)t->font->userdata.ptr;
                SpriteFont sprite_font = *font->font;
                DrawText((const char*)t->string, t->x, t->y, sprite_font.size, color);
//                DrawTextEx(sprite_font, (const char*)t->string, (Vector2){t->x, t->y}, sprite_font.size, 0, color);
            } break;
        }
    }

    nk_clear(&ctx);
}

static void
nk_raylib_handle_input()
{
    nk_input_begin(&ctx);

    int button = -1;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        button = MOUSE_LEFT_BUTTON;
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        button = MOUSE_RIGHT_BUTTON;
    } else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON) || IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) {
        button = MOUSE_MIDDLE_BUTTON;
    }

    // Gotta do the ol' switcheroo on button IDs
    //
    // raylib mouse buttons: left, middle, right
    // nuklear mouse buttons: left, right, middle
    if (button != -1) {
        int down = IsMouseButtonDown(button);
        if (button == MOUSE_RIGHT_BUTTON) {
            button = MOUSE_MIDDLE_BUTTON;
        } else if (button == MOUSE_MIDDLE_BUTTON) {
            button = MOUSE_RIGHT_BUTTON;
        }
        nk_input_button(&ctx, button, GetMouseX(), GetMouseY(), down);
    }

    float mouse_x = GetMouseX();
    float mouse_y = GetMouseY();

    if (mouse_x != prev_mouse_x || mouse_y != prev_mouse_y)
    {
        nk_input_motion(&ctx, mouse_x, mouse_y);
    }

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;


    nk_input_end(&ctx);
}

#endif //NUKLEAR_RAYLIB_H
