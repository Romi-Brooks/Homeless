-- 针对游戏内容默认的sfx，使用cpp native code进行加载，当玩家/开发者需要加载其余音乐，或是hot load的时候，使用lua
-- 简单的使用SFX.Load方式，这会使得当游戏启动时直接将文件加载到内存，开发者可以使用Package工具将资源文件打包到文件，然后使用Unpackage进行动态解包
-- 这将会减小内存的使用量，并且实现了lazy load

-- test sample
local lua_niaojiao = SFX.Load("lua_niaojiao", "Media/SFXS/Background_niaojiao.mp3")