screen = {
		w = love.graphics.getWidth(),
		h = love.graphics.getHeight()
	}
	
--VERSION AND SUCH
game_version = "0.2"
--love.graphics.setCaption("Nameless "..game_version)
	
--DEBUG RELATED
debug_enabled = true --debug mode enabled or not
debug_lines = false
debug_playermouse = false

slow = false



function love.load()
	font = {
		small = love.graphics.newFont(14),
		large = love.graphics.newFont(24)
	}
	
love.graphics.setFont(font.small)

	--REQUIRE
	require "menu"
	require "player"
	require "world"
	require "obj"
	require "ent"
	require "note"
	require "shaders"
	require "hud"
	require "game"
	--Game state
	state = {
			menu = "menu",
			game = "game"
		}
	gamestate = state.game
	
	--Creating HUD
	hud.newMeter("health", 12, screen.h - world.ground_h + 12, screen.w - 24, 10, player.health.m, {180, 0, 0}, 100)
	hud.newMeter("time", 12, screen.h - world.ground_h + 3, screen.w - 24, 5, game_time.m, {255, 255, 255}, 100)
	hud.newMeter("ammo", 12, screen.h - world.ground_h + 26, screen.w - 24, 5, player.ammo.m, {0, 180, 0}, 100)
	hud.newMeter("focus", screen.w - 12 - (player.focus.m * 2), screen.h - world.ground_h + 38, player.focus.m * 2, 10, player.focus.m, {0, 200, 255}, 100)
	
	--Game start
	game.reset()
	love.timer.step()
end

function love.update(dt)
	if slow then dt = dt / 4 end

	if gamestate == state.game then
		game.update(dt)
	elseif gamestate == state.menu then
		menu.update(dt)
	end
end

function love.draw()
	if gamestate == state.game then
		game.draw()
	elseif gamestate == state.menu then
		menu.draw()
	end
	
	if debug_enabled then
		--love.graphics.setColor(255, 255, 255, 255)
		--love.graphics.setFont(font.small)
		--[[love.graphics.print("FPS: "..love.timer.getFPS().."\nGame State: "..gamestate.."\nPlayer Friction: "..player.friction.c.."\nAcc: "..player.acc.c.."\nPlayer xVel: "..math.floor(player.xVel).."\nPlayer yVel: "..math.floor(player.yVel)
		.."\nPlayer airborne: "..tostring(player.air).."\nBullets: "..#bullets
		.."\nAngle: "..getAngle(player.x + (player.w / 2), player.y + (player.h / 2), love.mouse.getX(), love.mouse.getY())
		.."\nShaders Enabled: "..tostring(shaders_enabled), 12, 12)
		--]]
	end
	
end

function love.mousepressed(x, y, k)
	if gamestate == state.game then
		game.mousepressed(x, y, k)
	elseif gamestate == state.menu then
	
	end
end

function love.keypressed(key, unicode)
	if gamestate == state.game then
		game.keypressed(key)
	elseif gamestate == state.menu then
	
	end
	
	--DEBUG
	if key == "tab" then debug_enabled = not debug_enabled end --Debug Toggle
	if debug_enabled then --Debug keypresses
		if key == "escape" then
			love.event.push("quit")
		end
		if key == "o" then
			shaders_enabled = not shaders_enabled
			note.new("shaders_enabled: "..tostring(shaders_enabled), color_white)
		end
		if key == "n" then
			debug_playermouse = not debug_playermouse
			note.new("player_mouse: "..tostring(debug_playermouse), color_white)
		end
	end
end

--MATH FUNCTIONS
function getAngle(x1, y1, x2, y2) 
	return math.deg(math.atan2(x2-x1, y2-y1))
end

function checkCollision(ax1,ay1,aw,ah, bx1,by1,bw,bh)
	local ax2,ay2,bx2,by2 = ax1 + aw, ay1 + ah, bx1 + bw, by1 + bh
	return ax1 < bx2 and ax2 > bx1 and ay1 < by2 and ay2 > by1
end

function getDistance(x1, y1, x2, y2)
	local dx = x1-x2
	local dy = y1-y2
	return math.sqrt((dx * dx + dy * dy))
end