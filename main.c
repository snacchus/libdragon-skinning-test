#include <libdragon.h>
#include <GL/gl_integration.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <rspq_profile.h>

int main(void)
{
    resolution_t display_resolution = RESOLUTION_320x240;
    debug_init(DEBUG_FEATURE_LOG_USB | DEBUG_FEATURE_LOG_ISVIEWER);
    dfs_init(DFS_DEFAULT_LOCATION);
    display_init(display_resolution, DEPTH_16_BPP, 3, GAMMA_CORRECT_DITHER, FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    gl_init();

    model64_t *model = model64_load("rom:/Catherine_2.8.model64");
    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_resolution.width, display_resolution.height);

    model64_anim_play(model, "Walk", MODEL64_ANIM_SLOT_0, false, 0);
    model64_anim_set_loop(model, MODEL64_ANIM_SLOT_0, true);

    uint32_t frames = 0;
    rspq_profile_start();

    while (1) {
        // Using 1/60 seconds is not correct and results in FPS-dependent animation speed, but this is just for testing
        model64_update(model, 1.f/60.f);

        surface_t *display = display_get();
        rdpq_attach(display, &zbuffer);
        gl_context_begin();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, display_resolution.width/(float)display_resolution.height, 50.f, 300.f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 100, 250, 0, 100, 0, 0, 1, 0);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        for (size_t i = 0; i < 5; i++)
        {
            glPushMatrix();
            glTranslatef((i*80.f)-160.f, 0, 0);
            model64_draw(model);
            glPopMatrix();
        }

        gl_context_end();
        rdpq_detach_show();

        rspq_profile_next_frame();

        if (((frames++) % 60) == 0) {
            rspq_profile_dump();
            rspq_profile_reset();
        }
    }
}
