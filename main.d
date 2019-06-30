pragma(lib, "gtkd-3");

import gtk.Builder;
import gtk.Main;
import gtk.Window;
import std.stdio;

void main(string[] args)
{
    Main.init(args);
    Builder builder = new Builder();
    builder.addFromFile("design.glade");
    Window w = cast(Window)builder.getObject("main-window");
    w.addOnHide( delegate void(Widget){ Main.quit(); } );
    w.showAll();
    Main.run();
}
