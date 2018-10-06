package;

import openfl.display.Sprite;
import openfl.events.Event;
import openfl.system.System;

class Main extends Sprite 
{	
	public function new() 
	{
		super();
		Kartridge.initialize(null); //initialize first
		
		stage.addEventListener(Event.ENTER_FRAME, gameUpdate);
	}
	
	public function gameUpdate(e:Event):Void
	{
		Kartridge.update(); //call update every frame
		
		if (Kartridge.isReady()) //submit won't work until everything is connected
		{
			Kartridge.submit("high score", 100);
		}
	}
	
	//see Kartridge.hx for more methods you may need
}
