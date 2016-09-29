/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
Expression requireSystemClockConfig is undefined on line 51, column 8 in stm32xx_it_c.ftl.
The problematic instruction:
----------
==> if requireSystemClockConfig [on line 51, column 3 in stm32xx_it_c.ftl]
----------
Java backtrace for programmers:
----------
freemarker.core.InvalidReferenceException: Expression requireSystemClockConfig is undefined on line 51, column 8 in stm32xx_it_c.ftl.
at freemarker.core.TemplateObject.assertNonNull(TemplateObject.java:124)
at freemarker.core.Expression.isTrue(Expression.java:145)
at freemarker.core.ConditionalBlock.accept(ConditionalBlock.java:77)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.ConditionalBlock.accept(ConditionalBlock.java:79)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.Environment.visit(Environment.java:233)
at freemarker.core.CompressedBlock.accept(CompressedBlock.java:73)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:196)
at freemarker.core.Environment.process(Environment.java:176)
at freemarker.template.Template.process(Template.java:232)
at com.st.microxplorer.codegenerator.CodeEngine.freemarkerDo(CodeEngine.java:225)
at com.st.microxplorer.codegenerator.CodeEngine.genCode(CodeEngine.java:164)
at com.st.microxplorer.codegenerator.CodeGenerator.generateOutputCode(CodeGenerator.java:1707)
at com.st.microxplorer.codegenerator.CodeGenerator.generatePeriphConfigCode(CodeGenerator.java:1026)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCodeFiles(CodeGenerator.java:816)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCode(CodeGenerator.java:555)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.generateCode(ProjectBuilder.java:781)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createCode(ProjectBuilder.java:688)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createProject(ProjectBuilder.java:393)
at com.st.microxplorer.plugins.projectmanager.engine.GenerateProjectThread.run(GenerateProjectThread.java:37)