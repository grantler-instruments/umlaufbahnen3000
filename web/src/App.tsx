import { useEffect, useState } from "react";
import useStore from "./store";
import { Box, Button, Slider, TextField } from "@mui/material";

function App() {
  const rotation = useStore((state) => state.rotation);
  const tokens = useStore((state) => state.tokens);
  const removeToken = useStore((state) => state.removeToken);
  const addToken = useStore((state) => state.addToken);
  const toggle = useStore((state) => state.toggle);
  const setRpm = useStore((state) => state.setRpm);
  const rotateTrack = useStore((state) => state.rotateTrack);

  const [track, setTrack] = useState(0)

  useEffect(() => {
    toggle();
  }, []);

  const handleClick = (event: any, i: number) => {
    // Get the bounding box of the SVG
    const { left, top, width, height } = event.target.getBoundingClientRect();

    // Calculate the click's relative position
    const x = event.clientX - (left + width / 2); // Center X
    const y = event.clientY - (top + height / 2); // Center Y

    // Calculate the angle in radians
    let angle = Math.atan2(y, x);

    // Convert the angle to degrees and offset for 12 o'clock
    let degrees = (angle * (180 / Math.PI) + 90) % 360;
    if (degrees < 0) degrees += 360;

    console.log(`Rotation in degrees (12 o'clock start): ${degrees}`);
    addToken(i, degrees);
  };

  return (
    <Box display={"flex"} gap={3}>
      <svg
        xmlns="http://www.w3.org/2000/svg"
        viewBox="-170 -170 340 340"
        width="340"
        height="340"
      >
        {/* <circle cx="0" cy="0" r="1.5" fill="none" stroke="red" stroke-width="1" /> */}

        <circle
          cx="0"
          cy="0"
          r="170"
          fill="none"
          stroke="black"
          strokeWidth="1"
        />
        <rect
          x="-2"
          y="-140"
          width="4"
          height="140"
          fill="red"
          transform={`rotate(${rotation})`}
        />

        {Array(8)
          .fill(0)
          .map((_, i) => {
            return (
              <circle
                key={`circle-${i}`}
                cx="0"
                cy="0"
                r={22 + i * 20}
                stroke="rgba(0,255,0,0.6"
                strokeWidth="18"
                fill="none"
                onClick={(event) => {
                  handleClick(event, i);
                }}
              />
            );
          })}

        <circle
          cx="0"
          cy="0"
          r="3.6"
          fill="black"
          stroke="black"
          strokeWidth="1"
        />

        {tokens.map((token, i) => {
          const { track, rotation } = token; // Token's track and rotation
          const radius = 22 + track * 20; // Calculate the radius based on track
          const x = radius * Math.cos(((rotation - 90) * Math.PI) / 180); // X-coordinate
          const y = radius * Math.sin(((rotation - 90) * Math.PI) / 180); // Y-coordinate

          return (
            <circle
              key={`token-${i}`}
              cx={x}
              cy={y}
              r="8" // Token size
              fill="blue"
              onClick={() => {
                removeToken(token.id);
              }}
            />
          );
        })}
      </svg>
      <Box
        sx={{ backgroundColor: "pink" }}
        display={"flex"}
        flexDirection={"column"}
      >
        <Slider orientation="vertical"></Slider>
        <Box display={"flex"}>
        <TextField
        type="number" // Specify that this is a number input
        value={track}
        onChange={(event)=>setTrack(parseInt(event.target.value))}
        variant="outlined"
        label="track"
        InputProps={{
          inputMode: 'numeric', // Optional: specifies numeric input mode for better mobile experience
        }}
      />
      <Button variant="outlined" onClick={()=>rotateTrack(track, -1 )}>-</Button>
      <Button variant="outlined" onClick={()=>rotateTrack(track, 1 )}>+</Button>
      
        </Box>

        <Box display={"flex"}>
          <Button
            variant="outlined"
            onClick={() => {
              setRpm(33);
            }}
          >
            33
          </Button>
          <Button
            variant="outlined"
            onClick={() => {
              setRpm(4);
            }}
          >
            45
          </Button>
        </Box>
        <Button
          variant="outlined"
          fullWidth
          onClick={() => {
            toggle();
          }}
        >
          toggle
        </Button>
      </Box>
    </Box>
  );
}

export default App;
