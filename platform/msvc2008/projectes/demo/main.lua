function love.load()
	easing = require("libraries.easing")
	require("libraries.middleclass")
	vector = require("libraries.vector")
	
	require("scene.actor")
	require("scene.tools")
	require("scene.button")
	
	graphics = {}

	require("context")
	require("playscene")
	require("loading")
	require("mainmenu")
	require("mapselect")
	require("weaponselect")
	require("missionfail")
	require("missioncomplete")
	
	scenes = {
		loading = Loading(),
		mainmenu = MainMenu(),
		mapselect = MapSelect(),
		weaponselect = WeaponSelect(),
		play = PlayScene(),
		missionfail = MissionFail(),
		missioncomplete = MissionComplete(),
	}

	state = scenes.loading
	state:load()

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

function love.mousemoved(x, y)
	state:mousemoved(x, y)
end

function love.keypressed(key)
	state:keypressed(key)
end