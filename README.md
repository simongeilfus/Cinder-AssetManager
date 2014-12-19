Cinder-AssetManager
===================

*!New version is (here)[https://github.com/simongeilfus/Watchdog]!*

**Hot-Reloading:**

By default the asset will automaticaly be watched and reloaded on the fly.

```c++
AssetManager::load( "cinder_logo_alpha.png", [this](DataSourceRef dataSource){
    mTexture = gl::Texture::create( loadImage( dataSource ) );
} );
```

**Async:**

By modifying the AssetManger::Options passed through the load function you can
specify if you want asynchronous loading. In that case the loading will occur in
a separated thread, and will not block the main thread. Be carefull what you do
inside that thread.

```c++
AssetManager::load( "cinder_logo_alpha.png", [this](DataSourceRef dataSource){
    mSurface = Surface( loadImage( dataSource ) );
}, AssetManager::Options().asynchronous() );
```

Use the templated version if you want to pass an object from the loading thread to the main thread. Because we are providing the load function with two callbacks there's no need to specify Options().asynchronous() like we would do with only one callback.


```c++
// Load the image in a separated thread and returns the ImageSourceRef
auto asyncLoad = [](DataSourceRef dataSource){
    ImageSourceRef imageSource = loadImage( dataSource );
    return imageSource;
};
    
// The second callback is executed in the main thread so any OpenGL resources can be created here.
auto textureCreation = [this](ImageSourceRef imageSource){
    mTexture = gl::Texture::create( imageSource );
};

AssetManager::load<ImageSourceRef>( "cinder_logo_alpha.png", asyncLoad, textureCreation );
```

**File Grouping:**

```c++
AssetManager::load( "test.vert", "test.frag", [this]( DataSourceRef vert, DataSourceRef frag ) {
    // Load our shader and test if it is correctly compiled
    try {
        mShader = gl::GlslProg::create( vert, frag );
    }
    catch( gl::GlslProgCompileExc exc ){
        console() << exc.what() << endl;
    }
} );
```


Thanks to initializer lists and variadic templates we can group any number of assets
and have the same callback function called for initialization. Works the same way as the
non-variadic method with the only difference in the syntax that you can use {} to declare
your list of asset paths to load.
    
The only downside right now is that lambdas with a capture list can't be implicitly cast to
the variadic function pointer AssetManager is using, so the code looks a bit redundant,
because you need to specify twice the method signature ( cast + lambda parameters ).
    
```c++
AssetManager::load( { "img01.png", "img02.png", "img03.png", "img04.png", "img05.png" }, 
      (function<void(DataSourceRef,DataSourceRef,DataSourceRef,DataSourceRef,DataSourceRef)>) 
      [this](DataSourceRef img01, DataSourceRef img02, DataSourceRef img03, DataSourceRef img04, DataSourceRef img05) {
        
    // Do something (Texture atlas?)
} );
```
