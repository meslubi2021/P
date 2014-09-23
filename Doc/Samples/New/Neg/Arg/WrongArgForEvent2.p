event E:int;

main machine Entry {
    var m:machine;
    var a:any;

    start state foo {
        entry {
            m = new Foo();
            a = false;
            send m, E, a;
        }
    }
}

machine Foo {
    start state init {
        on E goto s1;
    }

    state s1 { }
}