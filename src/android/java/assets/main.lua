function love.load()
	easing = require("libraries.easing")
	require("libraries.middleclass")
	vector = require("libraries.vector")
	
	require("scene.actor")
	
	graphics = {}

	require("playscene")
	
--	scenes = {
--		scene0 = PlayScene(),
--	}
	state = PlayScene()
	
end

function love.draw()
	state:draw()
end

function love.update(dt)
	state:update(dt)
end

function love.mousepressed(x, y, button)
	state:mousepressed(x, y, button)
end

function love.mousereleased(x, y, button)
	state:mousereleased(x, y, button)
end

function love.keypressed(key)
	state:keypressed(key)
end
