import { defineStore } from 'pinia';
import { Sensor } from './Sensor';
import sleep from '../scripts/sleep';
import theme from 'tailwindcss/defaultTheme';

export const useAppStore = defineStore('appStore', {
  state: () => ({ themeIsDark: false }),

  actions: {
    init() {
      this.themeIsDark = localStorage.theme === 'dark';
      this.theme_set(this.themeIsDark);
      console.info("appStore setup")
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
