#if __emscripten__
#include <emscripten/fiber.h>
emscripten_fiber_t mm$fiber_js;
emscripten_fiber_t mm$fiber_mm;
char mm$fiber_js_astack[4096];
char mm$fiber_mm_astack[4096];
alignas(16) char mm$fiber_mm_stack[1048576];

void mm$start_mm_fiber(em_arg_callback_func mm$main) {
    emscripten_fiber_init_from_current_context(&mm$fiber_js, mm$fiber_js_astack, sizeof(mm$fiber_js_astack));
    emscripten_fiber_init(&mm$fiber_mm, mm$main, NULL, mm$fiber_mm_stack, sizeof(mm$fiber_mm_stack), mm$fiber_mm_astack, sizeof(mm$fiber_mm_astack));
    emscripten_fiber_swap(&mm$fiber_js, &mm$fiber_mm);
}
void mm$end_mm_fiber() {
    while(true) {
        emscripten_fiber_swap(&mm$fiber_mm, &mm$fiber_js);
    }
}
void mm$suspend_mm_fiber() {
    emscripten_fiber_swap(&mm$fiber_mm, &mm$fiber_js);
}
extern "C" EMSCRIPTEN_KEEPALIVE void resume_mm() {
    emscripten_fiber_swap(&mm$fiber_js, &mm$fiber_mm);
}
#endif
