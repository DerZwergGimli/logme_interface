import { defineStore } from 'pinia';
import { I_System } from './interfaces/I_System';
import { I_WiFi } from './interfaces/I_WiFi';
import { useSensorStore } from './SensorStore';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR } from '../scripts/toast_config';

export const useAppStore = defineStore('appStore', {
  state: () => {
    return {
      themeIsDark: false,
      system: {} as I_System | undefined,
      wifi: {} as I_WiFi | undefined,
    };
  },

  actions: {
    init() {
      this.themeIsDark = localStorage.theme === 'dark';
      this.theme_set(this.themeIsDark);
      console.info('appStore setup');
    },
    async fetch_wifi() {
      let wifi_json;
      let wifi_aps;

      await fetch(APP_API_URL + '/wifi')
        .then(resp => resp.json())
        .then(json => {
          wifi_json = json;
        });

      await fetch(APP_API_URL + '/ap')
        .then(resp => resp.json())
        .then(json => {
          wifi_aps = json;
        });

      this.wifi = {
        config: wifi_json,
        aps: wifi_aps,
      };
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
