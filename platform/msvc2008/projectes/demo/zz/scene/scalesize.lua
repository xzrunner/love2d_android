require("scene.action")

ScaleSize = class("ScaleSize", Action)

function ScaleSize:initialize(scale)
	Action.initialize(self)
	self.scale = scale
end

function ScaleSize:act(dt)
	if not self.finish then
		self.actor:setScale(self.scale)
		self.finish = true
	end
end