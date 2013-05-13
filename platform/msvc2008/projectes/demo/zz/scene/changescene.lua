require("scene.action")

ChangeScene = class("ChangeScene", Action)

function ChangeScene:initialize(scene)
	Action.initialize(self)
	
	self.scene = scene
end

function ChangeScene:act(dt)
	if not self.finish then
		state = scenes[self.scene]
		state:reset()
		self.finish = true
	end
end