# State Machine

Sample state machine representing a _Locked_, _Closed_, and _Open_ door. In each state, the _Lock_ or the _Handle_ may be tried.

```
Current State | Try    | Next State |
--------------|--------|------------|
Locked        | Lock   | Closed     |
Locked        | Handle | ERROR!     |
Closed        | Lock   | Locked     |
Closed        | Handle | Open       |
Open          | Lock   | ERROR!     |
Open          | Handle | Closed     |

```

## Driver Program

### C Code

```
    struct door *door = door_new();
    door_message(door);
    door_act(door, A_HANDLE);
    door_act(door, A_LOCK);
    door_act(door, A_LOCK);
    door_act(door, A_LOCK);
    door_act(door, A_HANDLE);
    door_act(door, A_LOCK);
    door_act(door, A_HANDLE);
    door_act(door, A_LOCK);
    door_free(door);
```

### Program Output

```
Door is Locked.
Cannot use Handle. Door is Locked.
Lock used. Door is now Closed.
Lock used. Door is now Locked.
Lock used. Door is now Closed.
Handle used. Door is now Open.
Cannot use Lock. Door is Open.
Handle used. Door is now Closed.
Lock used. Door is now Locked.
Leaving door Locked.
```

