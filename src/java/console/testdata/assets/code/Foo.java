// "/commented/line.png" is documentation
package a;
/* a block comment mentioning "/in/block.png"
   over two lines "/in/block2.png" */
class Foo {
    void f() {
        load("/gui/gui.png"); load("/terrain.png"); load("/misc/water.png");
        load("/gui/GUI.PNG");                 // upper-case extension: not a resource path
        load("/misc/tiny.png"); load( "/misc/tiny.png" );
        load("//double/slash.png");
        load("/lang/en_US.lang"); load("/missing/nothing.png"); load("/x.jpg"); load("/a-b_c/d.e.txt");
        load("/spaces in/name.png"); load("no/leading/slash.png"); load("/short.p"); load("/.png"); load("/a.png");
        String url = "http://example.com/skin.png"; load("/after/url.png");
        load("/cursor.png"); load("/shaders/post.fsh"); load("/font/default.png");
    }
}
