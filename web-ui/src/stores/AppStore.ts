import { defineStore } from 'pinia';
import { T_SensorTypes } from './interfaces/T_SensorTypes';
import { I_System } from './interfaces/I_System';

export const useAppStore = defineStore('appStore', {
  state: () => {
    return {
      themeIsDark: false,
      sensor_type: undefined as T_SensorTypes | undefined,
      system: {} as I_System | undefined,
    };
  },

  actions: {
    init() {
      this.themeIsDark = localStorage.theme === 'dark';
      this.sensor_type = T_SensorTypes.POWER;
      this.theme_set(this.themeIsDark);
      console.info('appStore setup');
    },
    theme_toggle() {
      this.themeIsDark = !this.themeIsDark;
      write_DarkMode_to_storage(this.themeIsDark);
      write_DarkMode_to_DOM(this.themeIsDark);
    },
    theme_set(theme: boolean) {
      this.themeIsDark = theme;
      write_DarkMode_to_storage(this.themeIsDark);
      write_DarkMode_to_DOM(this.themeIsDark);
    },
  },
  getters: {
    heapPercentage: state => {
      if (state.system?.info) {
        return (
          ((state.system?.info.free_heap ?? 0) /
            (state.system?.info.total_heap ?? 0)) *
          100
        );
      } else {
        return 0;
      }
    },
  },
});

function write_DarkMode_to_DOM(is_dark: boolean) {
  if (is_dark) {
    document.documentElement.classList.remove('dark');
  } else {
    document.documentElement.classList.add('dark');
  }
}

function write_DarkMode_to_storage(is_dark: boolean) {
  if (is_dark) {
    localStorage.theme = 'dark';
  } else {
    localStorage.theme = 'light';
  }
}
