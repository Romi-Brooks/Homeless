-- 针对游戏内容默认的music，使用cpp native code进行加载，当玩家/开发者需要加载其余音乐，或是hot load的时候，使用lua
-- 简单的使用Music.Load方式，这会使得当游戏启动时直接将文件加载到内存，开发者可以使用Package工具将资源文件打包到文件，然后使用Unpackage进行动态解包
-- 这将会减小内存的使用量，并且实现了lazy load

-- test sample
local lua_background_1 = Music.Load("lua_background_1", "Media/Musics/情歌悠扬 - 本兮.mp3")

local lua_background_2 = Music.Load("lua_background_2", "Media/Musics/滴滴 - 覆予.mp3")