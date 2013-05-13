Scene = class("Scene")

function Scene:initialize()
	self.actors = {}
	self.buttons = {}
end

function Scene:reset()
	for n,a in pairs(self.actors) do
		a:reset()
	end
	for n,b in pairs(self.buttons) do
		b:reset()
	end	
end

function Scene:draw()
	for n,a in pairs(self.actors) do
		a:draw()
	end
	for n,b in pairs(self.buttons) do
		b:draw()
	end
end

function Scene:update(dt)
	for n,a in pairs(self.actors) do
		a:update(dt)
	end
	for n,b in pairs(self.buttons) do
		b:update(dt)
	end
end

function Scene:mousepressed(x, y, button)
	for n,b in pairs(self.buttons) do
		b:mousepressed(x, y, button)
	end
end

function Scene:mousereleased(x, y, button)
	for n,b in pairs(self.buttons) do
		b:mousereleased(x, y, button)
	end
end
