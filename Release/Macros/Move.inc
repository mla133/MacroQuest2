|===================================================| 
|Move.Inc                                         | 
|                                                   | 
|Simple moving and object-avoidance routines        | 
|                                                   | 
|Originally by beatnik007 (Who credits Mckorr)      | 
|                                                   | 
|Revised and converted to MQ2Data by Terramantian   | 
|===================================================| 


| Sub MoveToLoc                                     | 
|---------------------------------------------------| 
|This simply moves the player to within 10 units of | 
|the requested location, while avoiding obstacles   | 
|                                                   | 
|This is beatnik007's original sub, with a few minor| 
|changes and rewritten in MQ2Data format            | 
|                                                   | 
|SYNTAX: /call MoveToLoc Y X                        | 


Sub MoveToLoc(MoveToY, MoveToX) 
   | /echo Moving to Location: ${MoveToY}, ${MoveToX}. 
   | /echo Distance: ${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]} 

    /declare running int local 
    /declare distanceNow float local 
    /declare distanceBefore float local 
    /declare distanceModifier int local 
    /varset running 0 
    /declare distanceTimer timer 0 
    /varset distanceModifier 1 
    
    :moveToLocation 

    /face fast nolook loc ${MoveToY},${MoveToX} 

    /if (${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]}<10) { 
        /keypress forward 
        /return 
    } 
    
    /if (${distanceTimer}==0) { 
        /if (${Me.Sneaking}) { 
            /varset distanceModifier 2 
        } else { 
            /varset distanceModifier 1 
        } 

        /varset distanceNow ${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]} 
        /if (${Math.Calc[${distanceBefore}-${distanceNow}]}<${Math.Calc[10/${distanceModifier}]} && ${distanceBefore} ) { 
      /call strafePastObstacle 
        } 
        /varset distanceBefore ${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]} 
        /varset distanceTimer 15 
    } 

    /if (${running}==0) { 
        /keypress forward 
        /if (${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]}>10) { 
            /varset running 1 
            /keypress forward hold 
        } 
    } else { 
        /if (${Math.Distance[${Me.Y},${Me.X}:${MoveToY},${MoveToX}]}<11) { 
            /varset running 0 
            /keypress forward 
        } 
    } 
    /goto :moveToLocation 
/return 


| Sub MoveToSpawn                                   | 
|---------------------------------------------------| 
|This moves the player to within a provided distance| 
|of the provided spawn, while avoiding obstacles    | 
|                                                   | 
|I "wrote" this, but as you can see, it's the same  | 
|as MoveToLoc with a few variable replacements.     | 
|                                                   | 
|The second parameter, Distance, allows for the     | 
|player to stop short of the target by a certain    | 
|amount, to provide for different aggro radii, etc. | 
|                                                   | 
|SYNTAX: /call MoveToSpawn ID Distance              | 

Sub MoveToSpawn(MoveToID, StopDistance) 

    /if (!(${Defined[MoveToID]})||(${Spawn[MoveToID].ID})) { 
        /echo Spawn ID not found or no ID provided. Aborting... 
        /return 
    } 
    /if (!(${Defined[StopDistance]})) { 
   /echo Stopping point not defined, using default distance of 70 
        /declare StopDistance int local 
        /varset StopDistance 70    
    } 

    /echo Moving to Spawn: ${MoveToID} (${Spawn[${MoveToID}].CleanName}). 
|   /echo Current Location: ${Spawn[${MoveToID}].Y}, ${Spawn[${MoveToID}].X} 
|   /echo Current Distance: ${Spawn[${MoveToID}].Distance} 

    /declare running int local 
    /declare distanceNow float local 
    /declare distanceBefore float local 
    /declare distanceModifier int local 
    /varset running 0 
    /declare distanceTimer timer 15 
    /varset distanceBefore ${Spawn[${MoveToID}].Distance} 
    /varset distanceModifier 1 
    
    :moveToSpawn 

    /squelch /face fast nolook id ${MoveToID} 

    /if (${Spawn[${MoveToID}].Distance}<${StopDistance}) { 
        /keypress forward 
        /return 
    } 
    
    /if (${distanceTimer}==0) { 
        /if (${Me.Sneaking}) { 
            /varset distanceModifier 2 
        } else { 
            /varset distanceModifier 1 
        } 

        /varset distanceNow ${Spawn[${MoveToID}].Distance} 
        /if (${Math.Calc[${distanceBefore}-${distanceNow}]}<${Math.Calc[10/${distanceModifier}]}) { 
      /call strafePastObstacle 
        } 
        /varset distanceBefore ${Spawn[${MoveToID}].Distance} 
        /varset distanceTimer 15 
    } 

    /if (${running}==0) { 
        /keypress forward 
        /if (${Spawn[${MoveToID}].Distance}>=${StopDistance}) { 
            /varset running 1 
            /keypress forward hold 
        } 
    } else { 
        /if (${Spawn[${MoveToID}].Distance}<${StopDistance}) { 
            /varset running 0 
            /keypress forward 
        } 
    } 
    /goto :moveToSpawn 
/return 

| Only to be used by the previous functions - It's obvious what it does. | 

sub strafePastObstacle 
    /keypress forward 
   | /keypress back hold 
   | /delay 5 
   | /keypress back 
    /if (${Math.Rand[99]}>50) { 
        /keypress strafe_right hold 
    } else { 
        /keypress strafe_left hold 
    } 
    /delay 1s 
    /keypress strafe_right 
    /keypress strafe_left 
    /keypress forward hold 
/return 