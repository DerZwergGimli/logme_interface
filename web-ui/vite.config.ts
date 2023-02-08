import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
// @ts-ignore
import UnoCSS from 'unocss/vite';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    UnoCSS({
      /* options */
    }),
  ],
  define: {
    APP_VERSION: JSON.stringify(process.env.npm_package_version),
    APP_API_URL: JSON.stringify(
      process.env.NODE_ENV === 'production' ? '' : 'http://localhost:3000'
    ),
  },
});
