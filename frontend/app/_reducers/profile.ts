import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { RootState } from "../store";

export interface StudentData {
  first_name: string;
  last_name: string;
  father_name: string;
  email: string;
  group_number: number;
  in_group_order: number;
}

export interface ProfileData {
  username: string;
  role: "student" | "admin";
  data: StudentData | null;
}

export interface ProfileState {
	loggedIn: boolean;
	profile: ProfileData | null
}

export const initialProfileState: ProfileState = {
	loggedIn: false,
	profile: null
};

export const profileStateSlice = createSlice({
  name: "Profile",
  initialState: initialProfileState,
  reducers: {
    login(
      state: ProfileState,
      action: PayloadAction<{profile: ProfileData}>,
    ) {
			state.loggedIn = true;
      state.profile = action.payload.profile;
    },
		logout(state, __) {
			state.loggedIn = false;
			state.profile = null;
		}
  },
});

export const SelectIsLoggedIn = (state: RootState) => state.profile.loggedIn;
export const SelectProfile = (state: RootState) => state.profile.profile;
export const {login, logout} = profileStateSlice.actions;

export default profileStateSlice.reducer;
