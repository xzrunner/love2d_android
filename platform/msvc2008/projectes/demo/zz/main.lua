function love.load()
	easing = require("libraries.easing")
	require("libraries.middleclass")
	vector = require("libraries.vector")
	
	require("scene.actor")
	require("scene.button")
	
	require("Scene0")
	require("Scene1")
	
	graphics = {}
	
	scenes = {
		scene0 = Scene0(),
		scene1 = Scene1(),
	}
	state = Scene0()
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
