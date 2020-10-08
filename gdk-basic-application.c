// gcc gdk-okno.c -Wall -g `pkg-config --cflags --libs gdk-3.0` -ogdk-okno

#include <gdk/gdk.h>

GMainLoop* main_loop;
GdkWindow* window;

void on_end_app()
{
	gdk_window_destroy(window);
	g_main_loop_quit(main_loop);
}

void on_expose(GdkEventExpose* ev)
{
	if (ev->region == NULL)
		return;
	GdkDrawingContext* dc = gdk_window_begin_draw_frame(
		ev->window, ev->region);
	gdk_window_end_draw_frame(ev->window, dc);
}

void on_gdk_event(GdkEvent* event, gpointer data)
{
	if (event->type == GDK_EXPOSE)
	{
		on_expose((GdkEventExpose*)event);
	}
	else if (event->type == GDK_DELETE)
	{
		on_end_app();
	}
}

int main(int argc, char** argv)
{
	GdkWindowAttr attributes;
	gint attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_TITLE | GDK_WA_WMCLASS;
	if (!gdk_init_check(&argc, &argv))
		return EXIT_FAILURE;
	memset(&attributes, 0, sizeof(attributes));
	attributes.window_type = GDK_WINDOW_TOPLEVEL;
	attributes.x = 100;
	attributes.y = 50;
	attributes.event_mask = GDK_ALL_EVENTS_MASK;
	attributes.width = 440;
	attributes.height = 480;
	attributes.title = "GDK základní okno";
	attributes.wclass = GDK_INPUT_OUTPUT;
	gdk_event_handler_set(on_gdk_event, NULL, NULL);
	window = gdk_window_new(NULL, &attributes, attr_mask);
	gdk_window_show(window);
	main_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(main_loop);
	g_main_loop_unref(main_loop);
	return EXIT_SUCCESS;
}
