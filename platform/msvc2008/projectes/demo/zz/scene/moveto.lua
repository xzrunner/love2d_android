require("scene.action")

MoveTo = class("MoveTo", Action)

function MoveTo:initialize(beginning, ending, during)
	Action.initialize(self)
	
	self.beginning = beginning
	self.ending = ending
	self.during = during
	self.time = 0
end

function MoveTo:act(dt)
	if self.time < self.during then
		self.time = self.time + dt

		local p = easing.linear(self.time, self.beginning, self.ending - self.beginning, self.during)
		self.actor:setPosition(p.x, p.y)
	else
		self.finish = true
	end
end

function MoveTo:reset()
	Action.reset(self)
	self.time = 0
end