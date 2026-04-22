export const add: (a: number, b: number) => number;
export const run: () => number;

import { NodeContent } from '@ohos.arkui.node';
export const createNativeNode: (content: NodeContent, tag: string) => void;

import resourceManager from '@ohos.resourceManager';
export const copyAssets: (resmgr: resourceManager.ResourceManager, path: string) => void;