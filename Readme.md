<h1>Crazy Code Documentation<h1>
<hr>
<h2>Source Code Structure<h2>
<ul>
    <li>cmake</li>
    <li>configs</li>
        <ul>
            <li>Contains configs</li>
        </ul>
    <li>docker</li>
        <ul>
            <li>Contains docker files</li>
        </ul>
    <li>include</li>
        <ul>
            <li>Contains header files</li>
            <li>Handler Factories: contains factories header in it</li>
        </ul>
    <li>src</li>
        <ul>
            <li>Contains source files</li>
            <li>Handler Factories: contains factories source in it</li>
        </ul>
    <li>static_files</li>
        <ul>
            <li>Contains different type of static files to be served</li>
        </ul>
    <li>tests</li>
        <ul>
            <li>Contains tests</li>
        </ul>
    <li>CMakeLists</li>
        <ul>
            <li>Contains tests</li>
        </ul>
    <li>Readme</li>
</ul>
<h2>How to build<h2>
    <ul>
        <li>First we need to change to the directory to the build <code>cd build</code></li>
        <li>To configure <code>cmake ..</code></li>
        <li>To build <code>make</code></li>
    </ul>
<h2>How to test<h2>
    <ul>
        <li>First we need to change to the directory to the build <code>cd build</code></li>
        <li>To test <code>make test</code></li>
    </ul>
<h2>How to run<h2>
    <ul>
        <li>First we need to change to the directory to the build <code>cd build</code></li>
        <li>To start the server <code> ./bin/server ./config/main_config </code></li>
    </ul>
