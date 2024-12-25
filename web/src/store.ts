import { create } from "zustand";
import { createJSONStorage, devtools, persist } from "zustand/middleware";
import packageJson from "../package.json";
import { v4 as uuidv4 } from 'uuid';
import { WebMidi } from "webmidi";

  WebMidi
  .enable()
  .then(onEnabled)
  .catch(err => alert(err));

function onEnabled() {
  
  // Inputs
  WebMidi.inputs.forEach(input => console.log(input.manufacturer, input.name));
  
  // Outputs
  WebMidi.outputs.forEach(output => console.log(output.manufacturer, output.name));

}

interface State {
  version: string;
  active: boolean;
  rpm: number;
  tokens: any[];
  rotation: number;
  toggle: () => void;
  addToken: (track: number, rotation: number) => void;
  removeToken: (id: string) => void;
  setRpm: (rpm: number) => void;
  rotateTrack: (track: number, rotation: number) => void;
}

let intervalId: any = null;

const useStore = create<State>()(
  devtools(
    persist(
      (set, get) => ({
        version: packageJson.version,
        active: false,
        rpm: 22,
        rotation: 0,
        tokens: [
          {
            id: uuidv4(),
            rotation: 90,
            track: 0,
          },
          {
            id: uuidv4(),
            rotation: 0,
            track: 0,
          },
          {
            id: uuidv4(),
            rotation: 180,
            track: 0,
          },
          {
            id: uuidv4(),
            rotation: 200,
            track: 1,
          },
        ],
        toggle: () => {
          const { active } = get();
          if (intervalId) {
            clearInterval(intervalId);
            intervalId = null;
            set({ active: false });
          } else {
            const {rpm} = get()
            const intervalTime = (60 / (rpm * 360)) * 1000;

            intervalId = setInterval(() => {
              const newRotation = (get().rotation + 1) % 360
              get().tokens.forEach((token) => {
                if(token.rotation === newRotation){
                  console.log(token.track)
                  WebMidi.outputs.filter((output) => output.name.includes("IAC")).forEach((output) => {
                    output.channels[16].playNote(36 + token.track, {duration: 100})
                  })
                // output.channels[11].playNote(60 + token.track);
                }
              })
              set((state) => ({ rotation: newRotation }));
            }, intervalTime);
            set({ active: true });

          }
        },
        addToken(track, rotation) {
          set((state) => ({
            tokens: [
              ...state.tokens,
              {
                id: uuidv4(),
                track,
                rotation: Math.round(rotation),
              },
            ],
          }));
        },
        removeToken(id) {
          set((state) => ({
            tokens: state.tokens.filter((token) => token.id !== id),
          }));
        },
        setRpm(rpm) {
          set({ rpm });
        },
        rotateTrack(track, rotation) {
          set((state) => ({
            tokens: state.tokens.map((token) =>
              token.track === track
                ? {
                    ...token,
                    rotation: (token.rotation + rotation) % 360,
                  }
                : token
            ),
          }));
        },
      }),
      {
        name: "app",
        storage: createJSONStorage(() => localStorage),
        partialize: (state) => {},
      }
    ),
    { name: "app" }
  )
);

export default useStore;
