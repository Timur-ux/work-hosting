import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { RootState } from "../store";

interface StudentData {
	first_name: string;
	last_name: string;
	father_name: string;
	email: string;
	group_number: number;
	in_group_number: number;
};

export interface ProfileState {
  username: string;
  role: "student" | "admin";
	data: StudentData | null;
};

export const initialProfileState: ProfileState | null = null;

export const profileStateSlice = createSlice({
  name: "Profile",
  initialState: initialProfileState,
  reducers: {
    login(state, action: PayloadAction<"LR" | "KP" | "Unset">) {
      state.workType = action.payload;
    },
  },
});

export const selectUserData = (state: RootState) => state.userData;
export const { workType, workNumber, gvName } = userDataSlice.actions;

export default userDataSlice.reducer;
