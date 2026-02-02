import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { RootState } from "../store";

export interface UserDataState {
  workType: "LR" | "KP" | "Unset";
  workNumber: number;
  gvName: string;
}

const initialState: UserDataState = {
  workType: "Unset",
  workNumber: -1,
  gvName: "Unset",
};

export const userDataSlice = createSlice({
  name: "UserData",
  initialState,
  reducers: {
    workType(state, action: PayloadAction<"LR" | "KP" | "Unset">) {
      state.workType = action.payload;
    },
    workNumber(state, action: PayloadAction<number>) {
      state.workNumber = action.payload;
    },
    gvName(state, action: PayloadAction<string>) {
      state.gvName = action.payload;
    },
  },
});

export const selectUserData = (state: RootState) => state.userData;
export const { workType, workNumber, gvName } = userDataSlice.actions;

export default userDataSlice.reducer;
