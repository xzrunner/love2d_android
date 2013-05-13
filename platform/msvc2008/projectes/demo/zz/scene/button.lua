require("scene.actor")

Button = class("Button", Actor)

function Button:initialize(image, x, y, a, sx, sy, w, h)
	Actor.initialize(self, image, x, y, a, sx, sy, w, h)
	
	self.press_actions = {}
	self.release_actions = {}
	self.update_actions = {}
end

function Button:update(dt)
	Actor.update(self, dt)
	
	for i, v in pairs(self.update_actions) do
		v:act(dt)
		if v.finish then
			table.remove(self.update_actions, i)
		end
	end
	
	self.hover = false
	
	local x = love.mouse.getX()
	local y = love.mouse.getY()
	
	if x > self.x - self.hWidth and x < self.x + self.hWidth and y > self.y - self.hHeight and y < self.y + self.hHeight then
		self.hover = true
	end
end

function Button:reset()
	Actor.reset(self)
	
	for i, v in pairs(self.press_actions) do
		v:reset()
	end
	for i, v in pairs(self.release_actions) do
		v:reset()
	end	
	
	for i, v in pairs(self.update_actions) do
		self.update_actions[i] = nil
	end
end

function Button:addPressAction(action)
	action.actor = self
	table.insert(self.press_actions, action)
end

function Button:addReleaseAction(action)
	action.actor = self
	table.insert(self.release_actions, action)
end

function Button:mousepressed(x, y, button)
	if self.hover then
		for i, v in pairs(self.press_actions) do
			table.insert(self.update_actions, v)
		end
		return true
	end
	
	return false
end

function Button:mousereleased(x, y, button)
	if self.hover then
		for i, v in pairs(self.release_actions) do
			table.insert(self.update_actions, v)
		end
		return true
	end
	
	return false
end
